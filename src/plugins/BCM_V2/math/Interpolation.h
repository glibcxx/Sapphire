#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

namespace interpolation {

    // A local implementation of Catmull-Rom spline interpolation for vectors to avoid the experimental spline header.
    inline glm::vec3 catmullRom(const glm::vec3 &p0, const glm::vec3 &p1, const glm::vec3 &p2, const glm::vec3 &p3, float t) {
        float t2 = t * t;
        float t3 = t2 * t;
        return 0.5f * ((2.0f * p1) + (-p0 + p2) * t + (2.0f * p0 - 5.0f * p1 + 4.0f * p2 - p3) * t2 + (-p0 + 3.0f * p1 - 3.0f * p2 + p3) * t3);
    }

    // A local implementation of cubic Bezier interpolation for vectors.
    inline glm::vec3 bezier(const glm::vec3 &p0, const glm::vec3 &p1, const glm::vec3 &p2, const glm::vec3 &p3, float t) {
        float u = 1.0f - t;
        float tt = t * t;
        float uu = u * u;
        float uuu = uu * u;
        float ttt = tt * t;

        glm::vec3 p = uuu * p0;
        p += 3.0f * uu * t * p1;
        p += 3.0f * u * tt * p2;
        p += ttt * p3;
        return p;
    }

    // Quaternion logarithm: maps a quaternion to a vector in the tangent space.
    inline glm::quat quat_log(const glm::quat &q) {
        glm::vec3 v(q.x, q.y, q.z);
        float     r = glm::length(v);

        if (r < std::numeric_limits<float>::epsilon()) {
            // If q is a real quaternion, the log is (0, 0, 0, 0)
            return glm::quat(0.0f, 0.0f, 0.0f, 0.0f);
        }

        float t = atan2(r, q.w);
        float s = t / r;
        return glm::quat(0.0f, s * q.x, s * q.y, s * q.z);
    }

    // Quaternion exponent: maps a vector from the tangent space back to the quaternion manifold.
    // Assumes input quaternion has w = 0.
    inline glm::quat quat_exp(const glm::quat &q) {
        glm::vec3 v(q.x, q.y, q.z);
        float     theta = glm::length(v);

        if (theta < std::numeric_limits<float>::epsilon()) {
            return glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
        }

        float c = cos(theta);
        float s = sin(theta) / theta;
        return glm::quat(c, s * q.x, s * q.y, s * q.z);
    }

    // Calculates an intermediate quaternion for the squad interpolation.
    inline glm::quat squad_intermediate(const glm::quat &q0, const glm::quat &q1, const glm::quat &q2) {
        glm::quat q1_inv = glm::inverse(q1);
        glm::quat term1 = quat_log(q1_inv * q0);
        glm::quat term2 = quat_log(q1_inv * q2);
        glm::quat sum = -(term1 + term2) / 4.0f;
        return q1 * quat_exp(sum);
    }

    // squad: Spherical Quadrangle Interpolation.
    inline glm::quat squad(const glm::quat &q1, const glm::quat &q2, const glm::quat &s1, const glm::quat &s2, float t) {
        float k = 2.0f * t * (1.0f - t);
        return glm::slerp(glm::slerp(q1, q2, t), glm::slerp(s1, s2, t), k);
    }

} // namespace interpolation