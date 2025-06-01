#pragma once

#include "SDK/api/src-deps/Core/Math/Frustum.h"
#include "MatrixStack.h"

namespace mce {

    // size: 512
    class Camera {
    public:
        MatrixStack viewMatrixStack;       // off+0
        MatrixStack worldMatrixStack;      // off+64
        MatrixStack projectionMatrixStack; // off+128

        glm::tmat4x4<float> mInverseViewMatrix; // off+192
        glm::tvec3<float>   mRight;             // off+256
        glm::tvec3<float>   mUp;                // off+268
        glm::tvec3<float>   mForward;           // off+280
        glm::tvec3<float>   mPosition;          // off+292
        float               mAspectRatio;       // off+304
        float               mFov;               // off+308
        float               mZNear;             // off+312
        float               mZFar;              // off+316
        Frustum             mFrustum;           // off+320
    };
    static_assert(sizeof(Camera) == 512);

} // namespace mce