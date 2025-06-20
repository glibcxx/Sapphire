#pragma once

#include <glm/glm.hpp>

namespace dragon::rendering {

    struct ScissorTestState {
        bool              mEnabled;     // off+0
        glm::tvec4<float> mScissorRect; // off+4
    };

}; // namespace dragon::rendering