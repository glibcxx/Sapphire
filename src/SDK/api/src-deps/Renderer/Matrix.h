#pragma once

#include <glm/glm.hpp>

// size: 64
class Matrix {
public:
    glm::tmat4x4<float> _m; // off+0
};
static_assert(sizeof(Matrix) == 64);