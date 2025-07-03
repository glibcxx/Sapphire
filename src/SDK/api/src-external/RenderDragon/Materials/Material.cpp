#include "Material.h"

namespace dragon::materials {

    Material *Material::ctor(
        CompiledMaterialDefinition                              &materialDefinition,
        const std::unordered_map<std::string, std::string>      &globalFlags,
        const std::optional<dragon::platform::GraphicsPlatform> &platform
    ) {
        using Hook = sapphire::ApiLoader<
            "\x4C\x8B\xDC\x53\x55\x56\x57\x41\x54\x41\x56\x41\x57\x48\x81\xEC\x00\x00\x00\x00\x48\x8B\x05\x00\x00\x00\x00\x48\x33\xC4\x48\x89\x84\x24\x00\x00\x00\x00\x49\x8B\xE9\x4D\x8B\xF0"_sig,
            &Material::ctor>;
        return (this->*Hook::origin)(materialDefinition, globalFlags, platform);
    }

} // namespace dragon::materials