#include "MaterialFilter.h"

std::optional<dragon::materials::ParameterId> DFC::MaterialFilter::setTexture(
    const dragon::materials::MaterialUniformName &propertyName,
    const dragon::ServerTexture                  &texture,
    const uintptr_t                               samplerFlags
) {
    using Hook = sapphire::ApiLoader<
        "\x48\x89\x5C\x24\x00\x57\x48\x83\xEC\x00\x4C\x8B\x59\x00\x49\x8B\xF9\x4C\x8B\xD1"_sig,
        &MaterialFilter::setTexture>;
    return (this->*Hook::origin)(propertyName, texture, samplerFlags);
}
