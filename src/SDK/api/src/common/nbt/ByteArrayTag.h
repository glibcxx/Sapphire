#pragma once

#include "Tag.h"

class ByteArrayTag : public Tag {
public:
    std::vector<unsigned char> data; // off+8

    ByteArrayTag() = default;

    ByteArrayTag(std::vector<unsigned char> data) :
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
