#pragma once

#include "SDK/api/src/common/entity/gamerefs_entity/EntityContext.h"

class Actor {
public:
    EntityContext mEntityContext; // off+8

    // ...

    virtual ~Actor() {}

    template <typename T>
    T *tryGetComponent() {
        return mEntityContext.mEnTTRegistry.template try_get<T>(mEntityContext.mEntity);
    }

    template <typename T>
    const T *tryGetComponent() const {
        return mEntityContext.mEnTTRegistry.template try_get<T>(mEntityContext.mEntity);
    }
};