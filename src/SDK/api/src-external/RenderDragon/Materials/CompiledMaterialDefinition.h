#pragma once

#include <variant>
#include "SDK/core/Core.h"

#include "Definition/MaterialDefinition.h"

namespace dragon::materials {

    // size: 336
    class CompiledMaterialDefinition {
    public:
        struct Pass {};

        std::unordered_map<std::string, Pass>                              mPasses;          // off+0
        std::string                                                        mName;            // off+64
        std::optional<std::string>                                         mParentName;      // off+96
        uint64_t                                                           mUnk128;          // off+128
        std::unordered_map<std::string, std::variant<std::monostate>>      mProperties;      // off+144
        std::unordered_map<std::string, definition::SamplerDefinition>     mIndexedSamplers; // off+208
        std::unordered_map<std::string, definition::CustomTypeDeclaration> mCustomTypes;     // off+272
    };

} // namespace dragon::materials