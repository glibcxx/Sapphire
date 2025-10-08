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

    const Tag *get(StringView name) const;

    Tag *get(StringView name);

    bool contains(StringView name, Tag::Type type) const {
        const Tag *tag = this->get(name);
        return tag && tag->getId() == type;
    }

    bool contains(StringView name) const {
        return this->get(name) != nullptr;
    }

    CompoundTagVariant &operator[](StringView name) { return mTags[std::string{name}]; }

    CompoundTagVariant &at(StringView name) { return mTags.at(std::string{name}); }

    const CompoundTagVariant &at(StringView name) const { return mTags.at(std::string{name}); }

    TagMap mTags; // off+8
};
static_assert(sizeof(CompoundTag) == 24);

#include "CompoundTagVariant.h"

inline const Tag *CompoundTag::get(StringView name) const {
    auto elem = this->mTags.find(name);
    if (elem != this->mTags.end())
        return elem->second.get();
    else
        return nullptr;
}

inline Tag *CompoundTag::get(StringView name) {
    auto elem = this->mTags.find(name);
    if (elem != this->mTags.end())
        return elem->second.get();
    else
        return nullptr;
}