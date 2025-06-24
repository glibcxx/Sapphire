#pragma once

#include <vector>
#include <memory>
#include "MaterialUniform.h"

namespace dragon::materials {

    class MaterialUniformHandles {
    public:
        std::vector<std::unique_ptr<dragon::materials::MaterialUniform>> mUniforms; // off+0
    };

} // namespace dragon::materials