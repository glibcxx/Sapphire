#pragma once

#include <memory>
#include "AssertResourceServiceErrorHandler.h"
#include "ResourcePointer.h"

namespace mce {

    template <typename ResourcePtr, typename ErrorHandler = mce::AssertResourceServiceErrorHandler>
    class ServerResourcePointer : ResourcePtr {
    public:
    };

} // namespace mce