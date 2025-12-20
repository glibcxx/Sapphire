#include "DefaultDataLoadHelper.h"

DefaultDataLoadHelper *DefaultDataLoadHelper::ctor() {
    auto vftbAddr = &__vftable;
    return (DefaultDataLoadHelper *)::memcpy((void *)this, &vftbAddr, sizeof(vftbAddr)); // NOLINT
}