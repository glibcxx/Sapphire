#pragma once

#include "ResourceBlockTemplate.h"
#include "SDK/api/src-external/RenderDragon/Resource/DragonBufferResourceService.h"

namespace mce {

    class PerFrameHandleTracker;

    // size: 24
    template <typename Resource>
    class ResourcePointer {
    public:
        std::shared_ptr<
            ResourceBlockTemplate<
                Resource,
                mce::PerFrameHandleTracker,
                dragon::BufferDescription>>
            mResourcePointerBlock;

        virtual ~ResourcePointer() {}
    };

} // namespace mce