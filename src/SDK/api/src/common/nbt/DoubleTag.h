#pragma once

#include "Tag.h"

class DoubleTag : public Tag {
public:
    double data = 0.0; // off+8

    DoubleTag() = default;

    DoubleTag(double data) :
        data(data) {}

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
