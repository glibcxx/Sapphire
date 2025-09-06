#pragma once

#include "SDK/core/ApiManager.h"

class ClientInputUpdateSystem {};

struct ClientInputUpdateSystemInternal {
    SDK_API /*virtual*/ void tick(void *context) /*override*/;
};