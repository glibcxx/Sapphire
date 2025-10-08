#pragma once

#include "SDK/api/src/common/world/level/block/actor/BlockActorRendererId.h"

class BlockActorRenderer;
class ActorResourceDefinitionGroup;
namespace mce {
    class TextureGroup;
}

// size: 40
class BlockActorRenderDispatcher {
public:
    using RendererMap = std::map<BlockActorRendererId, std::unique_ptr<BlockActorRenderer>>;
    using RendererIterator = RendererMap::iterator;
    using RendererCIterator = RendererMap::const_iterator;

    RendererMap mRenderers; // off+0
    std::vector<std::function<void(const ActorResourceDefinitionGroup &, std::shared_ptr<mce::TextureGroup>)>>
        mGameSpecificRegistrationCallbacks; // off+16
};