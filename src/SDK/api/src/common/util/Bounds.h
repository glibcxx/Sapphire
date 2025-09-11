#pragma once

#include "SDK/api/src/common/world/Pos.h"
#include "SDK/api/src/common/world/level/ChunkPos.h"

// size: 48
struct Bounds {
    // size: 32
    struct Iterator {
        Iterator(const Bounds &base, int startIdx) :
            mPos(base.mMin), mBounds(base), mIdx(startIdx) {}

        Pos           mPos;    // off+0
        const Bounds &mBounds; // off+16
        int           mIdx;    // off+24
    };

    Pos mMin{};      // off+0
    Pos mMax{};      // off+12
    Pos mDim{};      // off+24
    int mArea = 0;   // off+36
    int mSide = 0;   // off+44
    int mVolume = 0; // off+40

    Pos gridToChunk(const BlockPos &p) const { return {p.x >> 4, p.y >> 4, p.z >> 4}; }

    Bounds() = default;

    enum class Option : int {
        Default = 0,
        Flatten = 1,
    };

    Bounds(const BlockPos &minBlock, const BlockPos &maxBlock, int side, Bounds::Option buildOption) :
        mSide(side) {
        Pos min = this->gridToChunk(minBlock);
        Pos max = this->gridToChunk(maxBlock);
        if (buildOption == Option::Flatten) {
            max.y = 0;
            min.y = 0;
        }
        this->resize(min, max);
    }

    Bounds(const ChunkPos &min, const ChunkPos &max) :
        Bounds({min.x << 4, 0, min.z << 4}, {max.x << 4, 0, max.z << 4}, 16, Option::Flatten) {}

    void resize(const Pos &cmin, const Pos &cmax) {
        this->mMin = cmin;
        this->mMax = cmax;
        this->mDim = this->mMax - this->mMin + 1;
        this->mArea = this->mDim.z * this->mDim.x;
        this->mVolume = this->mDim.y * this->mArea;
    }
};
