#include "DefaultDataLoadHelper.h"
#include "SDK/core/ApiManager.h"

void *const *DefaultDataLoadHelper::__vftable = sapphire::loadVftable<
    "\x48\x8D\x05\x00\x00\x00\x00\x48\x89\x44\x24\x00\x4C\x8B\xF1"_sig,
    &DefaultDataLoadHelper::__vftable>();