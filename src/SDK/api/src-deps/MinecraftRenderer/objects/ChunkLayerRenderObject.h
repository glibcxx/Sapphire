#pragma once

namespace mce {
    class MaterialPtr;
}

// size: 40
struct ChunkLayerRenderObject {
    uint64_t                mChunkIdx;                 // off+0
    const mce::MaterialPtr *mMaterial;                 // off+8
    uint32_t                mIndicesStart;             // off+16
    uint32_t                mIndicesCount;             // off+20
    uint32_t                mUnsortedIndicesStart;     // off+24
    uint32_t                mUnsortedIndicesCount;     // off+28
    bool                    mShouldFallBackToUnsorted; // off+32
};