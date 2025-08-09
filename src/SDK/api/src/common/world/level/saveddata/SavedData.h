#pragma once

#include <string>

class CompoundTag;

// size: 48
class SavedData {
public:
    bool        mDirty; // off+8
    std::string mId;    // off+16

    // vtb+0
    virtual ~SavedData() = default;

    // vtb+1
    virtual void deserialize(const CompoundTag &) = 0;

    // vtb+2
    virtual void serialize(CompoundTag &) const = 0;
};