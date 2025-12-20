#include "EndTag.h"

EndTag *EndTag::ctor() {
    auto vftbAddr = &__vftable;
    return (EndTag *)::memcpy((void *)this, &vftbAddr, sizeof(vftbAddr)); // NOLINT
}