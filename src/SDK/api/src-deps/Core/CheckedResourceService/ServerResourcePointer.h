#pragma once

#include <memory>
#include "ResourcePointer.h"

namespace mce {

    // size: 24
    template <typename Resource>
    class ServerResourcePointer : public ResourcePointer<Resource> {};

} // namespace mce