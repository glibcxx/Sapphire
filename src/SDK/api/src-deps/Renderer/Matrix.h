#pragma once

#include <glm/glm.hpp>

// size: 64
class Matrix {
public:
    glm::tmat4x4<float> _m; // off+0

    operator glm::tmat4x4<float>() const { return _m; }
    operator glm::tmat4x4<float> &() { return _m; }
    operator glm::tmat4x4<float> const &() const { return _m; }
};
static_assert(sizeof(Matrix) == 64);