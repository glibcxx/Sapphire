#pragma once

#include "Tag.h"

class StringTag : public Tag {
public:
    std::string data; // off+8

    StringTag() = default;

    StringTag(std::string data) :
        data(std::move(data)) {}

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
