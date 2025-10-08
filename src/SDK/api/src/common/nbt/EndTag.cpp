#include "EndTag.h"
#include "SDK/core/ApiManager.h"

void *const *EndTag::__vftable = sapphire::loadVftable<
    "\x48\x8D\x05\x00\x00\x00\x00\x48\x89\x03\x48\x8B\xC3\xC6\x43\x00\x00\x48\x8B\x5C\x24"_sig,
    &EndTag::__vftable>();

EndTag *EndTag::ctor() {
    return (EndTag *)::memcpy((void *)this, &__vftable, sizeof(__vftable));
}