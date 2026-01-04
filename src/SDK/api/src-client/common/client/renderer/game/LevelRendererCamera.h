#pragma once

#include "SDK/api/src-deps/Core/Utility/optional_ref.h"
#include "SDK/api/src/common/world/actor/player/PlayerListener.h"
#include "SDK/api/src/common/world/level/LevelListener.h"
#include "SDK/api/src/common/world/level/SubChunkPos.h"
#include "common/Memory.hpp"

class BaseActorRenderContext;
class ScreenContext;
class IClientInstance;
class RenderChunkInstanced;
class TerrainMaterialVariationManager;
class TerrainLayer;
struct ViewRenderObject;
struct LevelRendererCommandListInit;
struct LevelRenderPreRenderUpdateParameters;
struct ChunkRenderObjectCollection;

class LevelRendererCamera {
public:
    /*virtual*/ Vec3 &getCameraPos() {
        /*
            search "rebuildClouds task" and find
            `v7.m128_f32[0] = (float)((float)(v8 + a3) * 0.02) + *(float *)(a1 + {here it is});`
            at the top
        */

#if MC_VERSION == v1_21_2
        return memory::getField<Vec3>(this, 1544);
#elif MC_VERSION == v1_21_50
        return memory::getField<Vec3>(this, 1560);
#elif MC_VERSION == v1_21_60
        return memory::getField<Vec3>(this, 1552);
#endif
    }

    SPHR_DECL_API("1.21.2,1.21.50,1.21.60", "call", "\xE8\x00\x00\x00\x00\xF3\x0F\x10\x00\x00\x66\x0F\x6E\x8B")
    SDK_API /*virtual*/ void preRenderUpdate(
        ScreenContext                        &screenContext,
        LevelRenderPreRenderUpdateParameters &levelRenderPreRenderUpdateParameters
    );

    SPHR_DECL_API("1.21.2", "\x40\x55\x53\x56\x57\x41\x54\x41\x55\x41\x56\x41\x57\x48\x8D\xAC\x24\x00\x00\x00\x00\x48\x81\xEC\x00\x00\x00\x00\x48\x8B\x05\x00\x00\x00\x00\x48\x33\xC4\x48\x89\x85\x00\x00\x00\x00\x4D\x8B\xE9\x4C\x89\x4D\x00\x49\x8B\xF0")
    SPHR_DECL_API("1.21.50,1.21.60", "\x40\x55\x53\x56\x57\x41\x54\x41\x55\x41\x56\x41\x57\x48\x8D\xAC\x24\x00\x00\x00\x00\x48\x81\xEC\x00\x00\x00\x00\x48\x8B\x05\x00\x00\x00\x00\x48\x33\xC4\x48\x89\x85\x00\x00\x00\x00\x4C\x89\x4D\x00\x4D\x8B\xE8\x48\x89\x54\x24")
    SDK_API /*virtual*/ void render(
        BaseActorRenderContext       &baseEntityRenderContext,
        const ViewRenderObject       &renderObj,
        IClientInstance              &ci,
        LevelRendererCommandListInit &levelRendererCommandListInit
    );

    SPHR_DECL_API("1.21.2,1.21.50", "call", "\xE8\x00\x00\x00\x00\x48\x8B\xC8\x48\x85\xC0\x75\x00\x48\xFF\xC3")
    SPHR_DECL_API("1.21.60", "call", "\xE8\x00\x00\x00\x00\x48\x8B\xC8\x48\x85\xC0\x75\x00\x48\xFF\xC7")
    SDK_API RenderChunkInstanced *getRenderChunkInstancedAt(const SubChunkPos &rcp) const;

    SPHR_DECL_API("1.21.2", "call", "\xE8\x00\x00\x00\x00\x49\x8B\xCF\x49\x8D\x47")
    SPHR_DECL_API("1.21.50,1.21.60", "call", "\xE8\x00\x00\x00\x00\x49\x8B\xD7\x49\x8D\x47")
    SDK_API void queueChunk(
        ChunkRenderObjectCollection                        &collection,
        const RenderChunkInstanced                         &renderChunkInstanced,
        float                                               farDistance2,
        float                                               currentTime,
        const TerrainMaterialVariationManager              &terrainVariationMgr,
        optional_ref<const TerrainMaterialVariationManager> fadeVariationMgr
    );

    SPHR_DECL_API("1.21.2", "\x48\x89\x5C\x24\x00\x48\x89\x74\x24\x00\x48\x89\x7C\x24\x00\x41\x57\x48\x83\xEC\x00\x4C\x8B\x94\x24")
    SPHR_DECL_API("1.21.50,1.21.60", "\x48\x89\x5C\x24\x00\x48\x89\x74\x24\x00\x57\x48\x83\xEC\x00\x4C\x8B\x9C\x24")
    SDK_API void _addToRenderChunkQueue(
        ChunkRenderObjectCollection           &collection,
        const TerrainMaterialVariationManager &terrainVariationMgr,
        const TerrainLayer                    &layer,
        const RenderChunkInstanced            &renderChunkInstanced,
        size_t                                 chunkIdx,
        const BlockPos                        &chunkPos
    );
};