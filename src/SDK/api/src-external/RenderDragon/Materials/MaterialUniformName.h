#pragma once

#include <unordered_map>

namespace dragon::materials {

    // size: 8
    struct MaterialUniformName {
        using HashType = uint64_t;

        HashType mHash; // off+0
    };

} // namespace dragon::materials

template <>
struct std::hash<dragon::materials::MaterialUniformName> {
    std::size_t operator()(const dragon::materials::MaterialUniformName &key) const {
        return key.mHash;
    }
};