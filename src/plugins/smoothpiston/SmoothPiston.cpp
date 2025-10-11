#include "SmoothPiston.h"

#include "SDK/api/sapphire/hook/Hook.h"
#include "SDK/api/sapphire/GUI/GUI.h"

#include "SDK/api/src/common/world/level/Level.h"
#include "SDK/api/src/common/world/level/BlockSource.h"
#include "SDK/api/src/common/world/level/block/actor/PistonBlockActor.h"
#include "SDK/api/src-client/common/client/renderer/blockactor/PistonBlockActorRenerer.h"
#include "SDK/api/src-client/common/client/renderer/blockactor/MovingBlockActorRenderer.h"
#include "SDK/api/src/common/dataloadhelper/DefaultDataLoadHelper.h"

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
    SmoothPistonPlugin::OnPistonUpdatePacketHook,
    PistonBlockActor,
    sapphire::hook::HookPriority::Normal,
    PistonBlockActor::_onUpdatePacket,
    void,
    const CompoundTag &data,
    BlockSource       &region
) {
    if (!plugin->mEnableSmoothPiston || region.mOwnerThreadID != plugin->mRenderThread)
        return this->origin(data, region);
    PistonState           oldState = this->mState;
    DefaultDataLoadHelper dataLoadHelper{};
    this->load(region.mLevel, data, dataLoadHelper);
    auto &needSpawnMB = memory::getField<bool>(&this->mSticky, 1);
    needSpawnMB = false;
    needSpawnMB = oldState != this->mState
               && (this->mState == PistonState::Expanding && this->mProgress == 0.0f
                   || this->mState == PistonState::Retracting && this->mProgress == 1.0f);
}

HOOK_TYPE(
    SmoothPistonPlugin::OnPistonSpawnMBHook,
    PistonBlockActor,
    sapphire::hook::HookPriority::Normal,
    PistonBlockActor::_spawnMovingBlock,
    void,
    BlockSource    &region,
    const BlockPos &blockPos
) {
    if (!plugin->mEnableSmoothPiston || region.mOwnerThreadID != plugin->mRenderThread)
        return this->origin(region, blockPos);
    if (auto blockActor = region.getBlockEntity(blockPos);
        blockActor && blockActor->mType == BlockActorType::MovingBlock) {
        auto mb = static_cast<const MovingBlockActor *>(blockActor);
        bool hasBlockActor = mb->mBlockEntity.get();
        if (!mb->mBlock)
            return this->origin(region, blockPos);
        Logger::Debug(
            "[OnPistonSpawnMB] piston {}, MB at {} spawned.",
            this->mPosition.toString(),
            blockPos.toString()
        );
        region.setBlock(blockPos, *mb->mBlock, 3, nullptr, nullptr);
        if (hasBlockActor) {
            blockActor = region.getBlockEntity(blockPos);
            if (!blockActor)
                return this->origin(region, blockPos);
            const_cast<BlockActor *>(blockActor)->mTerrainInterlockData.mRenderVisibilityState =
                ActorTerrainInterlockData::VisibilityState::Visible;
        }
    }
    this->origin(region, blockPos);
}

HOOK_TYPE(
    SmoothPistonPlugin::PistonActorTickHook,
    PistonBlockActor,
    sapphire::hook::HookPriority::Normal,
    PistonBlockActor::tick,
    void,
    BlockSource &region
) {
    bool &needSpawnMB = memory::getField<bool>(&this->mSticky, 1);
    if (needSpawnMB) {
        needSpawnMB = false;
        this->_sortAttachedBlocks(region);
        for (auto &&pos : this->mAttachedBlocks) {
            this->_spawnMovingBlock(region, pos);
        }
    }
    if (plugin->mEnableSmoothPiston && plugin->mRenderThread == region.mOwnerThreadID && this->mAttachedBlocks.size()) {
        if (this->mState == PistonState::Expanded || this->mState == PistonState::Retracted) {
            Logger::Debug(
                "[ClientPistonTick] piston at {}, {} blocks spawned.",
                this->mPosition.toString(),
                this->mAttachedBlocks.size()
            );
            this->_spawnBlocks(region);
            // for (auto &&pos : this->mAttachedBlocks) {
            //     auto blockEntity = region.getBlockEntity(pos);
            //     if (blockEntity && blockEntity->mType == BlockActorType::PistonArm) {
            //         const_cast<BlockActor *>(blockEntity)->mTerrainInterlockData.mRenderVisibilityState =
            //             ActorTerrainInterlockData::VisibilityState::Visible;
            //     }
            // }
        }
    }
    auto oldState = this->mState;
    auto lastp = this->mLastProgress;
    this->origin(region);
    if (plugin->mRenderThread == region.mOwnerThreadID && (oldState != this->mState || lastp != this->mLastProgress)) {
        Logger::Debug(
            "[ClientPistonTick] piston at {}, state from {} to {}.",
            this->mPosition.toString(),
            (int)oldState,
            (int)this->mState
        );
    }
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
            plugin->mCurrentOrder = 0;
        }
    }
}

HOOK_TYPE(
    SmoothPistonPlugin::SmoothMovingBlockHook,
    MovingBlockActorRenderer,
    sapphire::hook::HookPriority::Normal,
    MovingBlockActorRenderer::render,
    void,
    BaseActorRenderContext &context,
    BlockActorRenderData   &renderData
) {
    auto  &region = renderData.renderSource;
    auto  &movingBlock = static_cast<MovingBlockActor &>(renderData.entity);
    float &alpha = context.mFrameAlpha;
    if (plugin->mEnableSmoothPiston) {
        if (movingBlock.mTerrainInterlockData.mHasBeenDelayedDeleted) {
            movingBlock.mTerrainInterlockData.mRenderVisibilityState =
                ActorTerrainInterlockData::VisibilityState::DelayedDestructionNotVisible;
            return;
        } else {
            auto maybeMB = region.getBlockEntity(movingBlock.mPosition);
            if (maybeMB != &movingBlock) {
                movingBlock.mTerrainInterlockData.mHasBeenDelayedDeleted = true;
                if (maybeMB)
                    const_cast<BlockActor *>(maybeMB)->mTerrainInterlockData.mRenderVisibilityState =
                        ActorTerrainInterlockData::VisibilityState::Visible;
            }
        }
    }

    /*
        错时渲染的核心代码，newX表示活塞应该在什么时候开始伸出。
        由于游戏把推出动画分成两段进行，这段代码显得比较复杂，
        需要临时修改mLastProgress和mProgress以修改活塞的初末状态...反正比较复杂。
    */
    if (plugin->mEnablePistonTickOrderSeparator && plugin->mTotalTicked) {
        auto ownerPistonBlockActor = (PistonBlockActor *)region.getBlockEntity(movingBlock.mPistonPos);
        if (!ownerPistonBlockActor)
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
    sapphire::hook::HookPriority::Normal,
    PistonBlockActorRenderer::render,
    void,
    BaseActorRenderContext &context,
    BlockActorRenderData   &renderData
) {
    auto  &region = renderData.renderSource;
    auto  &pistonActor = static_cast<PistonBlockActor &>(renderData.entity);
    float &alpha = context.mFrameAlpha;
    plugin->mRenderThread = region.mOwnerThreadID;

    // 上面只会影响MovingBlock，这个是活塞臂

    if (plugin->mEnablePistonTickOrderSeparator && plugin->mTotalTicked) {
        float    oldAlpha = alpha;
        float    oldLastProgress = pistonActor.mLastProgress;
        float    oldProgress = pistonActor.mProgress;
        uint32_t gapReuseTickOrder = *(uint32_t *)((uintptr_t)&pistonActor.mTickCount + 4);
        float    newX = (float)gapReuseTickOrder / plugin->mTotalTicked * (plugin->mTotalTicked < 4 ? 0.45f : 0.65f);
        twoStageLerp(alpha, pistonActor, newStartX, newX);

        this->origin(context, renderData);
        alpha = oldAlpha;
        pistonActor.mLastProgress = oldLastProgress;
        pistonActor.mProgress = oldProgress;
    } else {
        this->origin(context, renderData);
    }
}

template <typename Ret, size_t VIndex>
class VCall_t {};

template <typename Ret, size_t VIndex>
constexpr VCall_t<Ret, VIndex> vCall;

template <typename Ret, typename T, size_t VIndex>
class VCall : public VCall_t<Ret, VIndex> {
    T *objPtr;

public:
    constexpr VCall(T *obj) :
        objPtr(obj) {}

    template <typename... Args>
    constexpr Ret operator()(Args &&...args) const {
        return memory::vCall<Ret>(objPtr, VIndex, std::forward<Args>(args)...);
    }
};

template <typename T, typename Ret, size_t VIndex>
constexpr auto operator->*(T *obj, VCall_t<Ret, VIndex>) {
    return VCall<Ret, T, VIndex>{obj};
}

template <typename T, typename Ret, size_t VIndex>
    requires(!std::is_pointer_v<std::remove_reference_t<T>>)
constexpr auto operator->*(T &obj, VCall_t<Ret, VIndex>) {
    return VCall<Ret, std::remove_reference_t<T>, VIndex>{std::addressof(obj)};
}

SmoothPistonPlugin::SmoothPistonPlugin() {
    plugin = this;
    if (!SmoothMovingBlockHook::hook())
        Logger::Error("[Better Piston & MovingBlock] SmoothMovingBlockHook 安装失败!");
    else
        Logger::Debug("[Better Piston & MovingBlock] SmoothMovingBlockHook 安装成功！");

    if (!SmoothPistonArmHook::hook())
        Logger::Error("[Better Piston & MovingBlock] SmoothPistonArmHook 安装失败!");
    else
        Logger::Debug("[Better Piston & MovingBlock] SmoothPistonArmHook 安装成功！");

    if (!PistonActorTickHook::hook())
        Logger::Error("[Better Piston & MovingBlock] PistonSeparatorHook 安装失败!");
    else
        Logger::Debug("[Better Piston & MovingBlock] PistonSeparatorHook 安装成功！");

    if (!OnPistonSpawnMBHook::hook())
        Logger::Error("[Better Piston & MovingBlock] OnPistonSpawnMBHook 安装失败！");

    if (!OnPistonUpdatePacketHook::hook())
        Logger::Error("[Better Piston & MovingBlock] OnPistonUpdatePacketHook 安装失败！");

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
