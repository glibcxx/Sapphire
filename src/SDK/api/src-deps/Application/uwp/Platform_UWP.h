#pragma once

#include "../CommonPlatform.h"
#include "SDK/api/src-deps/Core/Platform/ActivationArguments.h"

class AppPlatform_Winrt;

// size: 16
class UnkBase1463646A0 {
public:
    uint8_t mUnk8; // off+8

    // vtb+0
    virtual ~UnkBase1463646A0() = default;

    // vtb+1
    virtual void unk_pure_virtual_1() = 0;
};

namespace Bedrock {

    // size: 344 (1.21.2), 352 (1.21.50/1.21.60)
    class Platform_UWP : public CommonPlatform, public UnkBase1463646A0 {
    public:
        static void *const *__vftable0; // ??_7Platform_UWP@Bedrock@@6BCommonPlatform@Bedrock@@@

        ;                                                   // offset: (1.21.50/1.21.60), (1.21.2 = 1.21.50 - 8 bytes)
        std::unique_ptr<AppPlatform_Winrt> mAppPlatform;    // off+88
        bool                               mUnk96;          // off+96
        bool                               mUnk97;          // off+97
        bool                               mUnk98;          // off+98
        uint64_t                           mUnk104;         // off+104
        ActivationArguments                mActivationArgs; // off+112

        Platform_UWP *ctor(); // \x48\x89\x5C\x24\x00\x48\x89\x6C\x24\x00\x48\x89\x74\x24\x00\x57\x48\x83\xEC\x00\x48\x8B\xF9\x48\x89\x4C\x24\x00\x48\x8D\x59 (1.21.2)
                              // \x48\x89\x5C\x24\x00\x48\x89\x74\x24\x00\x57\x48\x83\xEC\x00\x48\x8B\x00\x48\x89\x4C\x24\x00\x33\xF6\x48\x89\x71\x00\x48\x89\x71\x00\x48\x89\x71\x00\x48\x89 (1.21.50/1.21.60)
    };
#if MC_VERSION == v1_21_2
    static_assert(sizeof(Platform_UWP) == 344);
#elif MC_VERSION == v1_21_50 || MC_VERSION == v1_21_60
    static_assert(sizeof(Platform_UWP) == 352);
#endif

} // namespace Bedrock
