#pragma once

class BlockSource;

class LevelChunk {
public:
    enum class Finalization : int {
        NeedsInstaticking = 0,
        NeedsPopulation = 1,
        Done = 2,
    };

    SPHR_DECL_API("1.21.2", "call",          "\xE8\x00\x00\x00\x00\x48\x8B\x77\x00\x48\x8D\x85")
    SPHR_DECL_API("1.21.50,1.21.60", "call", "\xE8\x00\x00\x00\x00\x4C\x8B\x76\x00\x48\x8D\x85")
    SDK_API void updateCachedData(BlockSource &region);
};