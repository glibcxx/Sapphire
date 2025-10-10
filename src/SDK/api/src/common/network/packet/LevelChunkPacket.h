#pragma once

#include "../Packet.h"
#include "SDK/api/src/common/world/level/ChunkPos.h"

// size: 128
class LevelChunkPacket : public Packet {
public:
    // size: 8
    struct SubChunkMetadata {
        static const uint32_t MAX_BLOBS = 17;

        uint64_t blobId; // off+0
    };

    bool                          mCacheEnabled;    // off+48
    ChunkPos                      mPos;             // off+52
    std::string                   mSerializedChunk; // off+64
    size_t                        mSubChunksCount;  // off+96
    std::vector<SubChunkMetadata> mCacheMetadata;   // off+104
};