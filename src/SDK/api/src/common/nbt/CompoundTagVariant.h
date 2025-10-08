#pragma once

#include <variant>
#include "DoubleTag.h"
#include "StringTag.h"
#include "ByteArrayTag.h"
#include "IntArrayTag.h"
#include "ShortTag.h"
#include "IntTag.h"
#include "Int64Tag.h"
#include "FloatTag.h"
#include "EndTag.h"
#include "ByteTag.h"
#include "ListTag.h"
#include "CompoundTag.h"

class CompoundTagVariant {
public:
    using Variant = std::variant<
        EndTag,
        ByteTag,
        ShortTag,
        IntTag,
        Int64Tag,
        FloatTag,
        DoubleTag,
        ByteArrayTag,
        StringTag,
        ListTag,
        CompoundTag,
        IntArrayTag>;

    Variant mTagStorage; // off+0

    CompoundTagVariant() = default;
    CompoundTagVariant(CompoundTagVariant &&other) = default;
    CompoundTagVariant &operator=(CompoundTagVariant &&other) = default;
    CompoundTagVariant(const CompoundTagVariant &) = delete;
    CompoundTagVariant &operator=(const CompoundTagVariant &) = delete;

    const Tag *operator->() const { return this->get(); }

    Tag *operator->() { return this->get(); }

    const Tag *get() const {
        return std::visit([](const auto &tag) -> const Tag * { return &tag; }, this->mTagStorage);
    }

    Tag *get() {
        return std::visit([](auto &tag) -> Tag * { return &tag; }, this->mTagStorage);
    }

    template <typename TAG>
    TAG *get() {
        auto tag = std::get_if<TAG>(&this->mTagStorage);
        if (tag)
            return tag;
        else
            return nullptr;
    }

    template <typename TAG>
    const TAG *get() const {
        auto tag = std::get_if<TAG>(&this->mTagStorage);
        if (tag)
            return tag;
        else
            return nullptr;
    }

    const Tag &operator*() const { return *this->get(); }

    Tag &operator*() { return *this->get(); }
};