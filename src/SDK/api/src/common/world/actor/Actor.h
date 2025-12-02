#pragma once

#include "SDK/api/src/common/ActorUniqueID.h"
#include "SDK/api/src/common/entity/gamerefs_entity/EntityContext.h"

class Actor {
public:
    EntityContext mEntityContext; // off+8

    // ...

    // vtb+0
    virtual ~Actor();

    SPHR_DECL_API("1.21.2", "\x40\x53\x48\x83\xEC\x00\x48\x8B\x05\x00\x00\x00\x00\x48\x33\xC4\x48\x89\x44\x24\x00\x8B\x41\x00\x48\x8D\x54\x24")
    SPHR_DECL_API("1.21.50,1.21.60", "\x40\x53\x48\x83\xEC\x00\x4C\x8B\x51\x00\xBB\x00\x00\x00\x00\x8B\x51")
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