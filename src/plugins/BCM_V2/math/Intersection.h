#pragma once

#include "SDK/api/src/common/world/phys/AABB.h"
#include <glm/glm.hpp>

namespace intersection {

    // Slab method for ray-AABB intersection
    inline bool rayAABB(const glm::vec3 &rayOrigin, const glm::vec3 &rayDir, const AABB &aabb, float &t) {
        glm::vec3 invDir = 1.0f / rayDir;
        glm::vec3 tMin = (aabb.min - rayOrigin) * invDir;
        glm::vec3 tMax = (aabb.max - rayOrigin) * invDir;

        glm::vec3 t1 = glm::min(tMin, tMax);
        glm::vec3 t2 = glm::max(tMin, tMax);

        float tNear = glm::max(glm::max(t1.x, t1.y), t1.z);
        float tFar = glm::min(glm::min(t2.x, t2.y), t2.z);

        if (tNear > tFar || tFar < 0.0f) {
            return false;
        }

        t = tNear < 0.0f ? tFar : tNear;
        return true;
    }

} // namespace intersection