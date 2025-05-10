#include "SmoothPiston.h"

#include "hook/Hook.hpp"

#include "logger/GameLogger.hpp"
#include "SDK/api/src/common/world/level/Level.h"
#include "SDK/api/src/common/world/level/BlockSource.h"
#include "SDK/api/src/common/world/level/block/actor/PistonBlockActor.h"
#include "SDK/api/src-client/common/client/renderer/actor/PistonBlockActorRenerer.h"

#include "SDK/api/sapphire/GUI/GUI.h"

static bool gEnableSmoothPiston = true;
static bool gEnablePistonTickOrderSeparator = true;

float easeInOut(float t) {
    return t < 0.5f ? 4.0f * t * t * t : 1.0f - std::pow(-2.0f * t + 2.0f, 3.0f) / 2.0f;
}

/*
    x < newX, y = 0;
    x >= newX, (newX, 0) -> (1, 1) 的一条直线
*/
constexpr float newStartX(float x, float newX) {
    if (x < newX)
        return 0.0f;
    return (x - newX) / (1.0f - newX);
}

// 由于一次推动动画被分成两个阶段进行插值，咱不得不写这么一大坨
template <typename Fn, typename... Args>
constexpr void twoStageLerp(float &alpha, PistonBlockActor &piston, Fn lerpFn, Args &&...args)
    requires(std::is_invocable_r_v<float, Fn, float, Args...>)
{
    bool   isSecondStage = piston.mLastProgress == 0.5f;
    float &lastProgress = piston.mLastProgress;
    float &progress = piston.mProgress;
    if (float delta = progress - lastProgress) {
        bool  expanding = delta > 0.0;
        float midPoint = lerpFn(0.5f, args...);
        (isSecondStage ? lastProgress : progress) = (expanding ? midPoint : (1.0f - midPoint));
        float globalT = isSecondStage ? (0.5f + alpha * 0.5f) : (alpha * 0.5f);
        float range = progress - lastProgress;
        alpha = std::min(
            isSecondStage
                ? (lerpFn(globalT, args...) - midPoint) / (1.0f - midPoint + std::numeric_limits<float>::epsilon())
                : lerpFn(globalT, args...) / (midPoint + std::numeric_limits<float>::epsilon()),
            1.0f
        );
    }
}

// ServerLevel和ClientLevel各有一个BlockActor
static std::thread::id gRenderThread{};

static uint32_t gCurrentOrder = 0;
static uint32_t gTotalTicked = 0;

HOOK_TYPE(PistonSeparatorHook, PistonBlockActor, PistonBlockActor::tick, void, BlockSource &region) {
    this->origin(region);
    if (gEnablePistonTickOrderSeparator) {
        // 复用一个gap用来存储本活塞的更新顺序
        uint32_t &gapReuseTickOrder = *(uint32_t *)((uintptr_t)&this->mTickCount + 4);

        /*
            mProgress表示目标阶段，mLastProgress上一阶段，它们的值只可能是 0.0 0.5 1.0，
            游戏会利用这两个值进行线性插值，渲染活塞动画，下面两个Hook里的alpha就是(水平)插值因子。

            mProgress等于0.5时表示活塞处于半伸出或半收回状态，此时游戏需要插值出前半段活塞动画，
            我们将tick顺序记在gapReuseTickOrder里。

            同一个方块实体在游戏内有两个实例，分别处于客户端和服务端，咱们通过所属线程排除掉其中一个。

            mLastProgress等于0.5则表示活塞处于已伸出或已收回状态，此时游戏需要插值出后半段活塞动画。
            这个时候也是非红石刻，我们清空gCurrentOrder和gRenderThread方便下一次使用。
        */
        if (this->mProgress == 0.5f) {
            if (gRenderThread == region.mOwnerThreadID)
                gapReuseTickOrder = gTotalTicked = gCurrentOrder++;
        } else if (this->mLastProgress == 0.5f) {
            gRenderThread = std::thread::id{};
            gCurrentOrder = 0;
        }
    }
}

HOOK_TYPE(
    SmoothMovingBlockHook,
    MovingBlockActorRenderer,
    MovingBlockActorRenderer::render,
    void,
    BaseActorRenderContext &context,
    BlockActorRenderData   &renderData
) {
    auto region = renderData.mRegion;
    auto movingBlock = (MovingBlockActor *)renderData.mBlockActor;
#if MC_VERSION == v1_21_50 || MC_VERSION == v1_21_60
    float &alpha = memory::getField<float>(context._fill, 208);
#elif MC_VERSION == v1_21_2
    float &alpha = memory::getField<float>(context._fill, 168);
#endif
    if (gEnableSmoothPiston) {
        /*
            提前返回可以终止本帧渲染。
            这段代码可以修复拖影问题，不要问为啥，反正这么写就是目前最佳方案。
        */
        if (movingBlock->mPreserved == true) {
            if (!movingBlock->mBlockEntity || movingBlock->mBlockEntity->mType != BlockActorType::PistonArm) {
                if (movingBlock->mPreservedLifespan <= 4 || alpha > 0.25f)
                    return;
            } else if (movingBlock->mPreservedLifespan <= 4)
                return;
        }
    }
    auto getBlockEntity = (const BlockActor *(*)(const IConstBlockSource *, const BlockPos &))region->vtable[4];
    auto ownerPistonBlockActor = (PistonBlockActor *)(getBlockEntity(region, movingBlock->mPistonPos));
    if (!ownerPistonBlockActor) // 这个的确可能是nullptr
        return this->origin(context, renderData);

    /*
        错时渲染的核心代码，newX表示活塞应该在什么时候开始伸出。
        由于游戏把推出动画分成两段进行，这段代码显得比较复杂，
        需要临时修改mLastProgress和mProgress以修改活塞的初末状态...反正比较复杂。
    */
    float oldAlpha = alpha;
    float oldLastProgress = ownerPistonBlockActor->mLastProgress;
    float oldProgress = ownerPistonBlockActor->mProgress;
    if (gEnablePistonTickOrderSeparator && gTotalTicked) {
        uint32_t gapReuseTickOrder = *(uint32_t *)((uintptr_t)&ownerPistonBlockActor->mTickCount + 4);
        float    newX = (float)gapReuseTickOrder / gTotalTicked * (gTotalTicked < 4 ? 0.45f : 0.65f); // 表示两个活塞最多不会相差0.45格或0.65格
        twoStageLerp(alpha, *ownerPistonBlockActor, newStartX, newX);

        // 如果同时想要缓入缓出，可以这么写：
        // twoStageLerp(alpha, *ownerPistonBlockActor, [](float a, float newX) { return easeInOut(newStartX(a, newX)); }, newX);
    }
    this->origin(context, renderData);
    alpha = oldAlpha;
    ownerPistonBlockActor->mLastProgress = oldLastProgress;
    ownerPistonBlockActor->mProgress = oldProgress;
    return;
}

HOOK_TYPE(
    SmoothPistonArmHook,
    PistonBlockActorRenderer,
    PistonBlockActorRenderer::render,
    void,
    BaseActorRenderContext &context,
    BlockActorRenderData   &renderData
) {
    auto  region = renderData.mRegion;
    auto  pistonActor = (PistonBlockActor *)renderData.mBlockActor;
    auto &pistonPos = *renderData.mPos;
#if MC_VERSION == v1_21_2
    float &alpha = memory::getField<float>(context._fill, 168);
#elif MC_VERSION == v1_21_50 || MC_VERSION == v1_21_60
    float &alpha = memory::getField<float>(context._fill, 208);
#endif

    if (gRenderThread == std::thread::id{})
        gRenderThread = ((BlockSource *)region)->mOwnerThreadID;

    // 上面只会影响MovingBlock，这个是活塞臂
    float oldAlpha = alpha;
    float oldLastProgress = pistonActor->mLastProgress;
    float oldProgress = pistonActor->mProgress;
    if (gEnablePistonTickOrderSeparator && gTotalTicked) {
        uint32_t gapReuseTickOrder = *(uint32_t *)((uintptr_t)&pistonActor->mTickCount + 4);
        float    newX = (float)gapReuseTickOrder / gTotalTicked * (gTotalTicked < 4 ? 0.45f : 0.65f);
        twoStageLerp(alpha, *pistonActor, newStartX, newX);
    }
    this->origin(context, renderData);
    alpha = oldAlpha;
    pistonActor->mLastProgress = oldLastProgress;
    pistonActor->mProgress = oldProgress;
    return;
}

void setEnableSmoothPiston(bool enable) {
    gEnableSmoothPiston = enable;
}

void installSmoothPiston() {
    if (!SmoothMovingBlockHook::hook())
        Logger::Error("[Better Piston & MovingBlock] SmoothMovingBlockHook 安装失败!");
    if (!SmoothPistonArmHook::hook())
        Logger::Error("[Better Piston & MovingBlock] SmoothPistonArmHook 安装失败!");
    if (!PistonSeparatorHook::hook())
        Logger::Error("[Better Piston & MovingBlock] PistonSeparatorHook 安装失败!");

    GuiOverlay::registerPluginSettings(
        {
            .name = "Better Piston & MovingBlock",
            .description = "Smooth Piston & MovingBlock, Visualize Tick Order",
            .drawSettings = []() {
                ImGui::Checkbox("Smooth Piston & MovingBlock", &gEnableSmoothPiston);
                ImGui::Checkbox("Visualize Tick Order", &gEnablePistonTickOrderSeparator);
            },
        }
    );
}

void uninstallSmoothPiston() {
    SmoothMovingBlockHook::unhook();
    SmoothPistonArmHook::unhook();
    PistonSeparatorHook::unhook();
}