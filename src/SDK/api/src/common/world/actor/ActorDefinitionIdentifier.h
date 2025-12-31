#pragma once

#include "SDK/api/src-deps/Core/Utility/StringHash.h"

// size: 176
struct ActorDefinitionIdentifier {
    std::string  mNamespace;     // off+0
    std::string  mIdentifier;    // off+32
    std::string  mInitEvent;     // off+64
    std::string  mFullName;      // off+96
    HashedString mCanonicalName; // off+128
};