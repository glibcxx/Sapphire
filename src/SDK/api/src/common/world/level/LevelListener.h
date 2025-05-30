#pragma once

#include "SDK/api/src/common/world/level/BlockSourceListener.h"
#include "SDK/api/src/common/world/phys/Vec3.h"
#include "SDK/api/src-deps/Core/Utility/StringHash.h"

class CompoundTag;
class ChunkSource;
class LevelChunk;
class MolangVariableMap;
struct ResolvedItemIconInfo;
enum class ParticleType : int;
enum class LevelEvent : int16_t;
namespace cg {
    class ImageBuffer;
}

// size: 8
class LevelListener : public BlockSourceListener {
public:
    // vtb+0
    virtual ~LevelListener() {}

    // vtb+9
    virtual void allChanged() = 0;

    // vtb+10
    virtual void sendServerLegacyParticle(ParticleType name, const Vec3 &pos, const Vec3 &dir, int data) = 0;

    // vtb+11
    virtual void addParticleEffect(
        const HashedString      &effect,
        const Actor             &actor,
        const HashedString      &locator,
        const Vec3              &emitterOffset,
        const MolangVariableMap &molangVariables
    ) = 0;

    // vtb+12
    virtual void addTerrainParticleEffect(
        const BlockPos &pos,
        const Block    &block,
        const Vec3     &emitterPosition,
        float           intensity,
        float           velocityScalar,
        float           emitterRadius
    ) = 0;

    // vtb+13
    virtual void addTerrainSlideEffect(
        const BlockPos &pos,
        const Block    &block,
        const Vec3     &emitterPosition,
        float           intensity,
        float           velocityScalar,
        float           emitterRadius
    ) = 0;

    // vtb+14
    virtual void addBreakingItemParticleEffect(
        const Vec3                 &pos,
        ParticleType                type,
        const ResolvedItemIconInfo &textureInfo
    ) = 0;

    // vtb+15
    virtual void playMusic(const std::string &name, const Vec3 &pos, float songOffset, float volume) = 0;

    // vtb+16
    virtual void playStreamingMusic(const std::string &name, int x, int y, int z) = 0;

    // vtb+17
    virtual void onEntityAdded(Actor &entity) = 0;

    // vtb+18
    virtual void onEntityRemoved(Actor &entity) = 0;

    // vtb+19
    virtual void onChunkLoaded(ChunkSource &source, LevelChunk &lc) = 0;

    // vtb+20
    virtual void onChunkReloaded(ChunkSource &source, LevelChunk &lc) = 0;

    // vtb+21
    virtual void onSubChunkLoaded(
        ChunkSource &source,
        LevelChunk  &lc,
        short        absoluteSubChunkIndex,
        bool         subChunkVisibilityChanged
    ) = 0;

    // vtb+22
    virtual void onChunkUnloaded(LevelChunk &lc) = 0;

    // vtb+23
    virtual void onLevelDestruction(const std::string &levelId) = 0;

    // vtb+24
    virtual void levelEvent(LevelEvent type, const Vec3 &pos, int data) = 0;

    // vtb+25
    virtual void levelEvent(LevelEvent type, const CompoundTag &data) = 0;

    // vtb+26
    virtual void takePicture(
        cg::ImageBuffer          &outImage,
        Actor                    *camera,
        Actor                    *target,
        struct ScreenshotOptions &screenshotOptions
    ) = 0;

    // vtb+27
    virtual void playerListChanged() = 0;

    // vtb+28
    virtual void onLevelDataCommandsChanged(bool change) = 0;

    // vtb+29
    virtual void onLevelDataWorldTemplateOptionsUnlocked() = 0;
};

static_assert(sizeof(LevelListener) == 8);