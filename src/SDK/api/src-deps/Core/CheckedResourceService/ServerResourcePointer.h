#pragma once

#include "pch.h" // IWYU pragma: keep

#include "ResourcePointer.h"

namespace mce {

    // size: 24
    template <typename Resource>
    class ServerResourcePointer : public ResourcePointer<Resource> {};

} // namespace mce