#pragma once

#include "SDK/api/src-deps/Core/Utility/StringHash.h"

class ActorRenderer;
class DataDrivenRenderer;
class ActorResourceDefinitionGroup;
namespace mce {
    class TextureGroup;
}

// size: 176
class ActorRenderDispatcher {
public:
    std::unordered_map<HashedString, std::unique_ptr<ActorRenderer>>      mNonDataDrivenRenderers; // off+8
    std::unordered_map<HashedString, std::unique_ptr<DataDrivenRenderer>> mDataDrivenRenderers;    // off+72
    std::vector<std::function<void(const ActorResourceDefinitionGroup &, mce::TextureGroup *)>>
             mGameSpecificRegistrationCallbacks; // off+136
    uint8_t  mClientSubId;                       // off+160
    uint64_t mUnk168;                            // off+168

    // vtb+0
    virtual ~ActorRenderDispatcher();
};