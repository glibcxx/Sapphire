#pragma once

#include "SDK/core/SymbolResolver.h"

class ClientInputUpdateSystem {};

struct ClientInputUpdateSystemInternal {
    SDK_API /*virtual*/ void tick(void *context) /*override*/;
};