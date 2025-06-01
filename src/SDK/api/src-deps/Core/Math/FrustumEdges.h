#pragma once

#include <glm/glm.hpp>

// size: 48
struct FrustumEdges {
    glm::tvec3<float> topLeft;     // off+0
    glm::tvec3<float> topRight;    // off+12
    glm::tvec3<float> bottomLeft;  // off+24
    glm::tvec3<float> bottomRight; // off+36
};
static_assert(sizeof(FrustumEdges) == 48);
