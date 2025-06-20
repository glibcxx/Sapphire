#pragma once

#include <glm/glm.hpp>
#include <variant>
#include "SDK/core/Core.h"

#include "Definition/MaterialDefinition.h"
#include "Definition/MaterialShared.h"

namespace dragon::materials {

    // size: 336
    class CompiledMaterialDefinition {
    public:
        struct Pass {};

        std::unordered_map<std::string, Pass> mPasses;     // off+0
        std::string                           mName;       // off+64
        std::optional<std::string>            mParentName; // off+96
        uint64_t                              mUnk128;     // off+128
        std::unordered_map<
            std::string,
            std::variant<
                std::monostate,
                definition::PropertyField<glm::tvec4<float>>,
                definition::PropertyField<glm::tmat3x3<float>>,
                definition::PropertyField<glm::tmat4x4<float>>,
                definition::ExternalUniformDeclaration>>
            mProperties; // off+144

        std::unordered_map<std::string, definition::SamplerDefinition>     mIndexedSamplers; // off+208
        std::unordered_map<std::string, definition::CustomTypeDeclaration> mCustomTypes;     // off+272

        void _loadFrom(std::istream &inStream, uint64_t a2, uint64_t a3, uint8_t a4);
    };

} // namespace dragon::materials