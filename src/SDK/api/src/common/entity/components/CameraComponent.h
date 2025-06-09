#pragma once

#include <glm/gtc/quaternion.hpp>
#include "SDK/api/src-deps/Core/Utility/StringHash.h"
#include "SDK/api/src-deps/Renderer/Matrix.h"

// size: 288
class CameraComponent {
public:
    HashedString    mId;                // off+0
    glm::qua<float> mOrientation;       // off+48
    glm::vec3       mPosition;          // off+64
    float           mAspectRation;      // off+76
    float           mFieldOfView;       // off+80
    float           mNearPlane;         // off+84
    float           mFarPlane;          // off+88
    Matrix          mPostViewTransform; // off+92
    Matrix          mSavedProjection;   // off+156
    Matrix          mSavedModelView;    // off+220
};
static_assert(sizeof(CameraComponent) == 288);