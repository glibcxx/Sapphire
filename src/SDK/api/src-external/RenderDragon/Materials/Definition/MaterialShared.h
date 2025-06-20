#pragma once

#include <optional>

namespace dragon::materials::definition {

    // size: 1
    struct ExternalUniformDeclaration {};

    template <typename Ty, typename Default = std::optional<Ty>>
    struct PropertyField {
        int     mArrayCount = 0; // off+0
        Default mDefaultValue;   // off+4

        bool mUnkFlagA = false;
        bool mUnkFlagB = false;
        bool mUnkFlagC = false;
    };

} // namespace dragon::materials::definition