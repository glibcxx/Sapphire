#pragma once

#include <memory>
#include <thread>

#include "IConstBlockSource.h"

class BlockSource : public IConstBlockSource, public std::enable_shared_from_this<BlockSource> {
public:
    const std::thread::id mOwnerThreadID;
    const bool            mAllowUnpopulatedChunks;
    const bool            mPublicSource;
    bool                  mCheckValidity;
    class Level          &mLevel;
};