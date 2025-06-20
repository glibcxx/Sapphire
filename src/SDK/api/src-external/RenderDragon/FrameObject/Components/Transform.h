#pragma once

#include <glm/glm.hpp>
#include <variant>
#include <vector>

namespace DFC /*dragon::frameobject::components*/ {

    // size: 72
    struct Transform {
        std::variant<glm::tmat4x4<float>, std::vector<glm::tmat4x4<float>>, uint64_t> mWorldMatrix; // off+0
    };

} // namespace DFC