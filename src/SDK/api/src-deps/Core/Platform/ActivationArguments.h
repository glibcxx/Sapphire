#pragma once

#include <unordered_map>
#include <string>
#include "SDK/api/src-deps/Core/Platform/PlatformTypes.h"

namespace Bedrock {

    enum class ActivationArgumentType : uint32_t {
        Launch = 0,
        Protocol = 1,
        File = 2,
    }; // Bedrock::ActivationArgumentType

    // size: 240
    class ActivationArguments {
    public:
        std::unordered_map<std::string, std::string> mArguments;                   // off+0
        std::string                                  mProcessRegistrationKey;      // off+64
        std::string                                  mUrl;                         // off+96
        ActivationArgumentType                       mType;                        // off+128
        uint32_t                                     mNumArgs;                     // off+132
        ARVRPlatform                                 mARVRPlatform;                // off+136
        bool                                         mGenerateDocumentation;       // off+140
        bool                                         mDisablePauseMenuOnFocusLost; // off+141
        uint8_t                                      mUnk142;                      // off+142
        std::string                                  mUnk144;                      // off+144
        std::string                                  mUnk176;                      // off+176
        std::string                                  mUnk208;                      // off+208

        ActivationArguments *ctor(); // \x48\x89\x5C\x24\x00\x48\x89\x4C\x24\x00\x57\x48\x83\xEC\x00\x48\x8B\xD9\xC7\x44\x24\x00\x00\x00\x00\x00\x48\x8D\x54\x24 (1.21.2)
                                     // \xE8\x00\x00\x00\x00\x90\x4C\x89\xB7\x00\x00\x00\x00\x48\x89\x3D (1.21.50/1.21.60)

        ActivationArguments &operator=(const ActivationArguments &other); // \x48\x89\x5C\x24\x00\x48\x89\x74\x24\x00\x57\x48\x83\xEC\x00\x48\x89\x6C\x24\x00\x48\x8B\xFA\x48\x8B\xD9\xE8 (1.21.50/1.21.60)
    };
    static_assert(sizeof(ActivationArguments) == 240);
} // namespace Bedrock