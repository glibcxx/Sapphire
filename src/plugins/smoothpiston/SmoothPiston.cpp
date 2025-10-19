#include "SmoothPiston.h"

#include "SDK/api/sapphire/hook/Hook.h"
#include "SDK/api/sapphire/GUI/GUI.h"

#include "SDK/api/src/common/dataloadhelper/DefaultDataLoadHelper.h"
#include "SDK/api/src/common/world/level/Level.h"
#include "SDK/api/src/common/world/level/BlockSource.h"
#include "SDK/api/src/common/world/level/block/Block.h"
#include "SDK/api/src/common/world/level/block/actor/PistonBlockActor.h"
#include "SDK/api/src-client/common/client/renderer/blockactor/PistonBlockActorRenerer.h"
#include "SDK/api/src-client/common/client/renderer/blockactor/MovingBlockActorRenderer.h"
#include "PistonLerpTools.h"

static SmoothPistonPlugin *plugin = nullptr;

class ClientPistonBlockActor : public PistonBlockActor {
public:
    enum class ClientPistonState : uint8_t {
        Retracted = 0,
        ExpandingNeeded = 1,
        Expanding = 2,
        Expanded = 3,
        RetractingNeeded = 4,
        Retracting = 5,
        Uninitialized = (uint8_t)-1,
    };

    static constexpr BlockPos ARM_DIRECTION_OFFSETS[6] = {
        {0,  -1, 0 },
        {0,  1,  0 },
        {0,  0,  1 },
        {0,  0,  -1},
        {1,  0,  0 },
        {-1, 0,  0 }
    };

    const BlockPos &getFacingDir(BlockSource &region) const {
        const Block   &pistonBlock = region.getBlock(this->mPosition);
        const FacingID face = pistonBlock.mData;
        return ARM_DIRECTION_OFFSETS[face];
    }

    BlockPos getTargetMovingBlockPosOffset(BlockSource &region) const {
        auto &facingDir = this->getFacingDir(region);
        return (this->mState == PistonState::Expanded || this->mState == PistonState::Expanding) ? facingDir : -facingDir;
    }

    void cacheSpawnMovingBlocks(BlockSource &region) {
        this->mFutureClientAttachedBlocks.reset();
        ::memset((void *)&this->mFutureClientAttachedBlocks, 0, sizeof(std::vector<const Block *>));
        auto &cachedBlocks = memory::getField<std::vector<const Block *>>(&this->mFutureClientAttachedBlocks, 0);
        cachedBlocks.reserve(this->mAttachedBlocks.size());
        for (auto &&pos : this->mAttachedBlocks) {
            auto actor = region.getBlockEntity(pos);
            if (actor && actor->mType == BlockActorType::MovingBlock) {
                auto mb = static_cast<const MovingBlockActor *>(actor);
                cachedBlocks.emplace_back(mb->mBlock);
            } else {
                auto &block = region.getBlock(pos);
                cachedBlocks.emplace_back(&block);
            }
        }
    }

    void spawnMovingBlocks(BlockSource &region) {
        size_t i = 0;
        auto  &cachedBlocks = memory::getField<std::vector<const Block *>>(&this->mFutureClientAttachedBlocks, 0);
        auto   tagetPosOffset = this->getTargetMovingBlockPosOffset(region);
        for (auto &&pos : this->mAttachedBlocks) {
            auto blockEntity = region.getBlockEntity(pos);
            if (blockEntity && blockEntity->mType == BlockActorType::MovingBlock) {
                auto mb = static_cast<const MovingBlockActor *>(blockEntity);
                auto maybePiston = region.getBlockEntity(mb->mPistonPos);
                if (maybePiston && maybePiston->mType == BlockActorType::PistonArm) {
                    auto piston = static_cast<const ClientPistonBlockActor *>(maybePiston);
                    piston->clearMovingBlocksRenderState(region);
                    const_cast<ClientPistonBlockActor *>(piston)->_spawnBlocks(region);
                }
            }
            auto &notair = region.getBlock(pos);
            this->_spawnMovingBlock(region, pos);
            blockEntity = region.getBlockEntity(pos + tagetPosOffset);
            if (blockEntity && blockEntity->mType == BlockActorType::MovingBlock && cachedBlocks.size()) {
                auto mb = static_cast<const MovingBlockActor *>(blockEntity);
                const_cast<MovingBlockActor *>(mb)->mBlock = cachedBlocks[i];
            }
            ++i;
        }
        std::vector<const Block *>{}.swap(cachedBlocks);
    }

    void clearMovingBlocksRenderState(BlockSource &region) const {
        const BlockPos tagetPosOffset = this->getTargetMovingBlockPosOffset(region);
        for (auto &&pos : this->mAttachedBlocks) {
            auto blockEntity = region.getBlockEntity(pos + tagetPosOffset);
            if (blockEntity && blockEntity->mType == BlockActorType::MovingBlock) {
                ((BlockActor *)blockEntity)->mTerrainInterlockData.mHasBeenDelayedDeleted = true;
                ((BlockActor *)blockEntity)->mTerrainInterlockData.mCreationTime = std::chrono::system_clock::now();
            }
        }
    }
};

HOOK_TYPE(
    PistonCtorHook,
    ClientPistonBlockActor,
    sapphire::hook::HookPriority::Normal,
    PistonBlockActor::ctor,
    PistonBlockActor *,
    const BlockPos &pos,
    bool            isSticky
) {
    auto &clientPistonState = memory::getField<ClientPistonState>(&this->mSticky, 1);
    auto &nextClientPistonState = memory::getField<ClientPistonState>(&this->mSticky, 2);
    clientPistonState = nextClientPistonState = ClientPistonState::Uninitialized;
    this->origin(pos, isSticky);
    this->mTerrainInterlockData.mRenderVisibilityState = ActorTerrainInterlockData::VisibilityState::Visible;
    return this;
}

HOOK_TYPE(
    SmoothPistonPlugin::PistonActorUpdatePacketHook,
    ClientPistonBlockActor,
    sapphire::hook::HookPriority::Normal,
    PistonBlockActor::_onUpdatePacket,
    void,
    const CompoundTag &data,
    BlockSource       &region
) {
    if (!plugin->mEnableSmoothPiston || !memory::vCall<bool>(&region.mLevel, 299))
        return this->origin(data, region);
    PistonState           oldState = this->mState;
    DefaultDataLoadHelper helper{};
    this->load(region.mLevel, data, helper);
    auto &clientPistonState = memory::getField<ClientPistonState>(&this->mSticky, 1);
    auto &nextClientPistonState = memory::getField<ClientPistonState>(&this->mSticky, 2);
    if (oldState != this->mState) {
        if (this->mState == PistonState::Expanding) {
            nextClientPistonState = clientPistonState = ClientPistonState::ExpandingNeeded;
            this->cacheSpawnMovingBlocks(region);
        } else if (this->mState == PistonState::Retracting) {
            nextClientPistonState = clientPistonState = ClientPistonState::RetractingNeeded;
            this->cacheSpawnMovingBlocks(region);
        }
    }
}

HOOK_TYPE(
    SmoothPistonPlugin::PistonActorTickHook,
    ClientPistonBlockActor,
    sapphire::hook::HookPriority::Normal,
    PistonBlockActor::tick,
    void,
    BlockSource &region
) {
    if (!plugin->mEnableSmoothPiston || !memory::vCall<bool>(&region.mLevel, 299))
        return this->origin(region);
    ++this->mTickCount; // BlockActor::tick

    auto &clientPistonState = memory::getField<ClientPistonState>(&this->mSticky, 1);
    auto &nextClientPistonState = memory::getField<ClientPistonState>(&this->mSticky, 2);

    if (clientPistonState == ClientPistonState::Uninitialized) {
        if (this->mState == PistonState::Expanded || this->mState == PistonState::Expanding)
            clientPistonState = nextClientPistonState = ClientPistonState::Expanded;
        else if (this->mState == PistonState::Retracted || this->mState == PistonState::Retracting)
            clientPistonState = nextClientPistonState = ClientPistonState::Retracted;
        this->mFutureClientAttachedBlocks.reset();
    }

    const BlockPos &facingDir = this->getFacingDir(region);
    this->mBB = AABB::fromPoints({this->mPosition}, {this->mPosition + facingDir});
    this->mBB.max += {1, 1, 1};

    uint32_t &tickOrder = memory::getField<uint32_t>(&this->mTickCount, 4);

    clientPistonState = nextClientPistonState;
    switch (clientPistonState) {
    case ClientPistonState::Retracted:
        this->mProgress = 0.0f;
        this->mLastProgress = 0.0f;
        this->mState = this->mNewState = PistonState::Retracted;
        this->clearMovingBlocksRenderState(region);
        this->_spawnBlocks(region);
        break;
    case ClientPistonState::ExpandingNeeded:
        tickOrder = plugin->mTotalTicked = plugin->mCurrentOrder++;
        nextClientPistonState = ClientPistonState::Expanding;
        this->mProgress = 0.5f;
        this->mLastProgress = 0.0f;
        this->mState = this->mNewState = PistonState::Expanding;
        this->spawnMovingBlocks(region);
        break;
    case ClientPistonState::Expanding:
        plugin->mCurrentOrder = 0;
        nextClientPistonState = ClientPistonState::Expanded;
        this->mProgress = 1.0f;
        this->mLastProgress = 0.5f;
        this->mState = this->mNewState = PistonState::Expanded;
        break;
    case ClientPistonState::Expanded:
        this->mProgress = 1.0f;
        this->mLastProgress = 1.0f;
        this->mState = this->mNewState = PistonState::Expanded;
        this->clearMovingBlocksRenderState(region);
        this->_spawnBlocks(region);
        break;
    case ClientPistonState::RetractingNeeded:
        tickOrder = plugin->mTotalTicked = plugin->mCurrentOrder++;
        nextClientPistonState = ClientPistonState::Retracting;
        this->mProgress = 0.5f;
        this->mLastProgress = 1.0f;
        this->mState = this->mNewState = PistonState::Retracting;
        this->spawnMovingBlocks(region);
        break;
    case ClientPistonState::Retracting:
        plugin->mCurrentOrder = 0;
        nextClientPistonState = ClientPistonState::Retracted;
        this->mProgress = 0.0f;
        this->mLastProgress = 0.5f;
        this->mState = this->mNewState = PistonState::Retracted;
        break;
    default:
        break;
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
    using namespace std::chrono_literals;
    auto  &movingBlock = static_cast<MovingBlockActor &>(renderData.entity);
    auto  &region = renderData.renderSource;
    float &alpha = context.mFrameAlpha;
    if (plugin->mEnableSmoothPiston) {
        auto  maybeMB = renderData.renderSource.getBlockEntity(movingBlock.mPosition);
        auto &interlock = movingBlock.mTerrainInterlockData;
        if (interlock.mHasBeenDelayedDeleted) {
            if (std::chrono::system_clock::now() - interlock.mCreationTime >= 35ms)
                interlock.mRenderVisibilityState =
                    ActorTerrainInterlockData::VisibilityState::DelayedDestructionNotVisible;
        } else if (maybeMB != &movingBlock) {
            interlock.mHasBeenDelayedDeleted = true;
            interlock.mCreationTime = std::chrono::system_clock::now();
        }
    }

    if (plugin->mEnablePistonTickOrderSeparator && plugin->mTotalTicked) {
        auto ownerPistonBlockActor = (PistonBlockActor *)region.getBlockEntity(movingBlock.mPistonPos);
        if (!ownerPistonBlockActor)
            return this->origin(context, renderData);
        float oldAlpha = alpha;
        float oldLastProgress = ownerPistonBlockActor->mLastProgress;
        float oldProgress = ownerPistonBlockActor->mProgress;
        auto &tickOrder = memory::getField<uint32_t>(&ownerPistonBlockActor->mTickCount, 4);
        float newX = (float)tickOrder / plugin->mTotalTicked * (plugin->mTotalTicked < 4 ? 0.45f : 0.65f); // 表示两个活塞最多不会相差0.45格或0.65格
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

    if (plugin->mEnablePistonTickOrderSeparator && plugin->mTotalTicked) {
        float oldAlpha = alpha;
        float oldLastProgress = pistonActor.mLastProgress;
        float oldProgress = pistonActor.mProgress;
        auto &tickOrder = memory::getField<uint32_t>(&pistonActor.mTickCount, 4);
        float newX = (float)tickOrder / plugin->mTotalTicked * (plugin->mTotalTicked < 4 ? 0.45f : 0.65f);
        twoStageLerp(alpha, pistonActor, newStartX, newX);

        this->origin(context, renderData);
        alpha = oldAlpha;
        pistonActor.mLastProgress = oldLastProgress;
        pistonActor.mProgress = oldProgress;
    } else {
        this->origin(context, renderData);
    }
}

SmoothPistonPlugin::SmoothPistonPlugin() {
    plugin = this;
    if (!SmoothMovingBlockHook::hook())
        Logger::Error("[SmoothPiston] SmoothMovingBlockHook 安装失败!");
    if (!SmoothPistonArmHook::hook())
        Logger::Error("[SmoothPiston] SmoothPistonArmHook 安装失败！");
    if (!PistonActorTickHook::hook())
        Logger::Error("[SmoothPiston] PistonSeparatorHook 安装失败!");
    if (!PistonActorUpdatePacketHook::hook())
        Logger::Error("[SmoothPiston] PistonActorUpdatePacketHook 安装失败!");
    if (!PistonCtorHook::hook())
        Logger::Error("[SmoothPiston] PistonCtorHook 安装失败!");

    GuiOverlay::registerPluginSettings(
        {
            .name = "Better Piston",
            .description = "Better Piston, Visualize Tick Order",
            .drawSettings = [this]() {
                ImGui::Checkbox("Smooth Piston", &this->mEnableSmoothPiston);
                ImGui::Checkbox("Visualize Tick Order", &this->mEnablePistonTickOrderSeparator);
            },
        }
    );
}

SmoothPistonPlugin::~SmoothPistonPlugin() {
    SmoothMovingBlockHook::unhook();
    SmoothPistonArmHook::unhook();
    PistonActorTickHook::unhook();
    PistonActorUpdatePacketHook::unhook();
    PistonCtorHook::unhook();
    plugin = nullptr;
}

SmoothPistonPlugin &SmoothPistonPlugin::getInstance() {
    static SmoothPistonPlugin s{};
    return s;
}
