#pragma once

#include "SDK/api/src-deps/Core/File/Path.h"

// size: 8
class IDynamicPackageConsumer {
public:
    // vtb+0
    virtual ~IDynamicPackageConsumer();

    // vtb+1
    virtual void setDynamicPackagePath(Core::HeapPathBuffer path) = 0;
};