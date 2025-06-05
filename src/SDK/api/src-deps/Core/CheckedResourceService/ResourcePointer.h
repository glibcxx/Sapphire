#pragma once

#include "ResourceBlockTemplate.h"

namespace mce {

    template <typename Resource, typename Block, template <typename T = Block> class Ptr = std::shared_ptr>
    class ResourcePointer {
    public:
        Ptr<Block> ptr;

        virtual ~ResourcePointer() {}
    };

} // namespace mce