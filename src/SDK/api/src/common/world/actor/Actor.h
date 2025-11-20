#pragma once

#include "SDK/api/src/common/ActorUniqueID.h"
#include "SDK/api/src/common/entity/gamerefs_entity/EntityContext.h"

class Actor {
public:
    EntityContext mEntityContext; // off+8

    // ...

    // vtb+0
    virtual ~Actor();

    SDK_API const ActorUniqueID &getOrCreateUniqueID() const;

    template <typename T>
    T *tryGetComponent() {
        return mEntityContext.mEnTTRegistry.template try_get<T>(mEntityContext.mEntity);
    }

    template <typename T>
    const T *tryGetComponent() const {
        return mEntityContext.mEnTTRegistry.template try_get<T>(mEntityContext.mEntity);
    }
};