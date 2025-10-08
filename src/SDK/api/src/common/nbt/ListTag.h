#pragma once

#include "Tag.h"

class CompoundTag;

class ListTag : public Tag {
public:
    using List = std::vector<std::unique_ptr<Tag>>;

    List      mList{};           // off+8
    Tag::Type mType = Type::End; // off+32

    ListTag() = default;

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
};