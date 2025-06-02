#include "SmoothPiston.h"

#include "SDK/api/sapphire/hook/Hook.h"

#include "logger/GameLogger.hpp"
#include "SDK/api/src/common/world/level/Level.h"
#include "SDK/api/src/common/world/level/BlockSource.h"
#include "SDK/api/src/common/world/level/block/actor/PistonBlockActor.h"
#include "SDK/api/src-client/common/client/renderer/actor/PistonBlockActorRenerer.h"

#include "SDK/api/sapphire/GUI/GUI.h"

static SmoothPistonPlugin *plugin = nullptr;

static float easeInOut(float t) {
    return t < 0.5f ? 4.0f * t * t * t : 1.0f - std::pow(-2.0f * t + 2.0f, 3.0f) / 2.0f;
}

/*
    x < newX, y = 0;
    x >= newX, (newX, 0) -> (1, 1) 的一条直线
*/
static constexpr float newStartX(float x, float newX) {
    if (x < newX)
        return 0.0f;
    return (x - newX) / (1.0f - newX);
}

// 由于一次推动动画被分成两个阶段进行插值，咱不得不写这么一大坨
template <typename Fn, typename... Args>
static constexpr void twoStageLerp(float &alpha, PistonBlockActor &piston, Fn lerpFn, Args &&...args)
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

HOOK_TYPE(
    SmoothPistonPlugin::PistonActorTickHook,
    PistonBlockActor,
    hook::HookPriority::Normal,
    PistonBlockActor::tick,
    void,
    BlockSource &region
) {
    this->origin(region);
    if (plugin->mEnablePistonTickOrderSeparator) {
        // 复用一个gap用来存储本活塞的更新顺序
        uint32_t &gapReuseTickOrder = *(uint32_t *)((uintptr_t)&this->mTickCount + 4);

        /*
            mProgress表示目标阶段，mLastProgress上一阶段，它们的值只可能是 0.0 0.5 1.0，
            游戏会利用这两个值进行线性插值，渲染活塞动画，下面两个Hook里的alpha就是(水平)插值因子。

            mProgress等于0.5时表示活塞处于半伸出或半收回状态，此时游戏需要插值出前半段活塞动画，
            我们将tick顺序记在gapReuseTickOrder里。

            同一个方块实体在游戏内有两个实例，分别处于客户端和服务端，咱们通过所属线程排除掉其中一个。

            mLastProgress等于0.5则表示活塞处于已伸出或已收回状态，此时游戏需要插值出后半段活塞动画。
            这个时候也是非红石刻，我们清空CurrentOrder和RenderThread方便下一次使用。
        */
        if (this->mProgress == 0.5f) {
            if (plugin->mRenderThread == region.mOwnerThreadID)
                gapReuseTickOrder = plugin->mTotalTicked = plugin->mCurrentOrder++;
        } else if (this->mLastProgress == 0.5f) {
            plugin->mRenderThread = std::thread::id{};
            plugin->mCurrentOrder = 0;
        }
    }
    if (plugin->mRenderThread == region.mOwnerThreadID) {
        // if (this->mPosition.x > 0 && this->mPosition.x < 4)
        //     Logger::Debug("{}, {}", this->mPosition.toString(), (int)this->mTerrainInterlockData.mRenderVisibilityState);
        this->mTerrainInterlockData.mRenderVisibilityState = ActorTerrainInterlockData::VisibilityState::Visible;
    }
}

HOOK_TYPE(
    SmoothPistonPlugin::SmoothMovingBlockHook,
    MovingBlockActorRenderer,
    hook::HookPriority::Normal,
    MovingBlockActorRenderer::render,
    void,
    BaseActorRenderContext &context,
    BlockActorRenderData   &renderData
) {
    auto region = renderData.renderSource;
    auto movingBlock = (MovingBlockActor *)renderData.entity;
    float &alpha = context.mFrameAlpha;
    if (plugin->mEnableSmoothPiston) {
        /*
            提前返回可以终止本帧渲染。
            一行代码（大概）修复拖影问题，不要问为啥，反正这么写就是目前最佳方案。
        */
        if (movingBlock->mPreserved == true) return;
        /*
            这一行可以干掉拖影，但是会有别的问题，也就是活塞被另一个活塞推就位时会短暂不渲染活塞臂，
            这个问题在慢速情况下会得到缓解，因为其不受游戏内Timer计时器控制，活塞臂消失的时间不会变化，
            解决办法是前面 PistonActorTickHook 中插入 mRenderVisibilityState 置 Visible 的代码
        */
    }

    /*
        错时渲染的核心代码，newX表示活塞应该在什么时候开始伸出。
        由于游戏把推出动画分成两段进行，这段代码显得比较复杂，
        需要临时修改mLastProgress和mProgress以修改活塞的初末状态...反正比较复杂。
    */
    if (plugin->mEnablePistonTickOrderSeparator && plugin->mTotalTicked) {
        auto ownerPistonBlockActor = memory::vCall<PistonBlockActor *>(region, 4, movingBlock->mPistonPos);
        if (!ownerPistonBlockActor) // 这个的确可能是nullptr
            return this->origin(context, renderData);
        float    oldAlpha = alpha;
        float    oldLastProgress = ownerPistonBlockActor->mLastProgress;
        float    oldProgress = ownerPistonBlockActor->mProgress;
        uint32_t gapReuseTickOrder = *(uint32_t *)((uintptr_t)&ownerPistonBlockActor->mTickCount + 4);
        float    newX = (float)gapReuseTickOrder / plugin->mTotalTicked * (plugin->mTotalTicked < 4 ? 0.45f : 0.65f); // 表示两个活塞最多不会相差0.45格或0.65格
        twoStageLerp(alpha, *ownerPistonBlockActor, newStartX, newX);

        // 如果同时想要缓入缓出，可以这么写：
        // twoStageLerp(alpha, *ownerPistonBlockActor, [](float a, float newX) { return easeInOut(newStartX(a, newX)); }, newX);

        this->origin(context, renderData);
        alpha = oldAlpha;
        ownerPistonBlockActor->mLastProgress = oldLastProgress;
        ownerPistonBlockActor->mProgress = oldProgress;
        return;
    } else {
        this->origin(context, renderData);
    }
}

HOOK_TYPE(
    SmoothPistonPlugin::SmoothPistonArmHook,
    PistonBlockActorRenderer,
    hook::HookPriority::Normal,
    PistonBlockActorRenderer::render,
    void,
    BaseActorRenderContext &context,
    BlockActorRenderData   &renderData
) {
    auto region = renderData.renderSource;
    auto pistonActor = (PistonBlockActor *)renderData.entity;
    float &alpha = context.mFrameAlpha;
    if (plugin->mRenderThread == std::thread::id{})
        plugin->mRenderThread = region->mOwnerThreadID;

    // 上面只会影响MovingBlock，这个是活塞臂

    if (plugin->mEnablePistonTickOrderSeparator && plugin->mTotalTicked) {
        float    oldAlpha = alpha;
        float    oldLastProgress = pistonActor->mLastProgress;
        float    oldProgress = pistonActor->mProgress;
        uint32_t gapReuseTickOrder = *(uint32_t *)((uintptr_t)&pistonActor->mTickCount + 4);
        float    newX = (float)gapReuseTickOrder / plugin->mTotalTicked * (plugin->mTotalTicked < 4 ? 0.45f : 0.65f);
        twoStageLerp(alpha, *pistonActor, newStartX, newX);

        this->origin(context, renderData);
        alpha = oldAlpha;
        pistonActor->mLastProgress = oldLastProgress;
        pistonActor->mProgress = oldProgress;
    } else {
        this->origin(context, renderData);
    }
}

SmoothPistonPlugin::SmoothPistonPlugin() {
    plugin = this;
    if (!SmoothMovingBlockHook::hook())
        Logger::Error("[Better Piston & MovingBlock] SmoothMovingBlockHook 安装失败!");
    if (!SmoothPistonArmHook::hook())
        Logger::Error("[Better Piston & MovingBlock] SmoothPistonArmHook 安装失败!");
    if (!PistonActorTickHook::hook())
        Logger::Error("[Better Piston & MovingBlock] PistonSeparatorHook 安装失败!");

    GuiOverlay::registerPluginSettings(
        {
            .name = "Better Piston & MovingBlock",
            .description = "Smooth Piston & MovingBlock, Visualize Tick Order",
            .drawSettings = [this]() {
                ImGui::Checkbox("Smooth Piston & MovingBlock", &this->mEnableSmoothPiston);
                ImGui::Checkbox("Visualize Tick Order", &this->mEnablePistonTickOrderSeparator);
            },
        }
    );
}

SmoothPistonPlugin::~SmoothPistonPlugin() {
    SmoothMovingBlockHook::unhook();
    SmoothPistonArmHook::unhook();
    PistonActorTickHook::unhook();
    plugin = nullptr;
}

SmoothPistonPlugin &SmoothPistonPlugin::getInstance() {
    static SmoothPistonPlugin s{};
    return s;
}
