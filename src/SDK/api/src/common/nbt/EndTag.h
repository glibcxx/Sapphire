#pragma once

#include "Tag.h"

class EndTag : public Tag {
public:
    SDK_API static void *const *__vftable;

    SPHR_API EndTag();
#pragma SPHR_LINKER_SYM_ALIAS("??0EndTag@@QEAA@XZ", "?ctor@EndTag@@QEAAPEAV1@XZ")

    SPHR_API EndTag *ctor();

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
