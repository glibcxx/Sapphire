#pragma once

#include "FrustumEdges.h"

// size: 192
class Frustum {
public:
    glm::tvec4<float> mFrustumPlanes[6];       // off+0
    FrustumEdges      mNearClippingPlaneEdges; // off+96
    FrustumEdges      mFarClippingPlaneEdges;  // off+144

    void calculateFrustum(const glm::tmat4x4<float> &mvp);
};
static_assert(sizeof(Frustum) == 192);