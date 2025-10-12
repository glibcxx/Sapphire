#pragma once

#include "Vec3.h"

class AABB {
public:
    Vec3 min, max;

    static constexpr AABB fromPoints(const Vec3 &A, const Vec3 &B) {
        return {Vec3::min(A, B), Vec3::max(A, B)};
    }
};