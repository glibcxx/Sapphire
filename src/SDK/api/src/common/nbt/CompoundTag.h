#pragma once

#include <map>
#include "Tag.h"

class CompoundTagVariant;

using TagMap = std::map<std::string, CompoundTagVariant, std::less<void>>;

// size: 24
class CompoundTag : public Tag {
public:
    using StringView = std::string_view;

    CompoundTag() = default;
    CompoundTag(CompoundTag &&) = default;
    CompoundTag &operator=(CompoundTag &&) = default;
    CompoundTag(const CompoundTag &) = default;
    CompoundTag &operator=(const CompoundTag &) = default;

    // vtb+2
    virtual void write(IDataOutput &) const override;

    // vtb+3
    virtual void load(IDataInput &) override;

    // vtb+4
    virtual std::string toString() const override;

    // vtb+5
    virtual Type getId() const override;

    // vtb+9
    virtual Unique<Tag> copy() const override;

    // vtb+10
    virtual size_t hash() const override;

    TagMap mTags; // off+8
};
static_assert(sizeof(CompoundTag) == 24);