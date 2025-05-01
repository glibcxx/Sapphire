#include "ClientInstance.h"

void *const *ClientInstance::__vftable0 = reinterpret_cast<void *const *>(
    core::getImagebase()
#if MC_VERSION == v1_21_2
    + 0x4CBCB68
#elif MC_VERSION == v1_21_50
    + 0x6382D58
#elif MC_VERSION == v1_21_60
    + 0x6C34DF8
#endif
);

void *const *ClientInstance::__vftable1 = reinterpret_cast<void *const *>(
    core::getImagebase()
#if MC_VERSION == v1_21_2
    + 0x4CBD920
#elif MC_VERSION == v1_21_50
    + 0x6383B60
#elif MC_VERSION == v1_21_60
    + 0x6C34D78
#endif
);

void *const *ClientInstance::__vftable2 = reinterpret_cast<void *const *>(
    core::getImagebase()
#if MC_VERSION == v1_21_2
    + 0x4CBD948
#elif MC_VERSION == v1_21_50
    + 0x6382CE0
#elif MC_VERSION == v1_21_60
    + 0x6C34DA0
#endif
);

void *const *ClientInstance::__vftable3 = reinterpret_cast<void *const *>(
    core::getImagebase()
#if MC_VERSION == v1_21_2
    + 0x4CBD9A0
#elif MC_VERSION == v1_21_50
    + 0x6382D38
#elif MC_VERSION == v1_21_60
    + 0x6C36A38
#endif
);

ClientInstance *ClientInstance::primaryClientInstance{};
