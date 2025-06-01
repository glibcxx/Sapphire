#pragma once

#include "ResourceBlockTemplate.h"

namespace mce {

    template <typename Resource, typename Block, typename Ptr = std::shared_ptr<Block>>
    class ResourcePointer {
    public:
        Ptr ptr;

        virtual ~ResourcePointer() {}
    };

} // namespace mce