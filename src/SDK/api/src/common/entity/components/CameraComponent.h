#pragma once

#include <glm/gtc/quaternion.hpp>
#include "SDK/api/src-deps/Core/Utility/StringHash.h"
#include "SDK/api/src-deps/Renderer/Matrix.h"

class CameraComponent {
public:
    HashedString    mId;
    glm::qua<float> mOrientation;
    glm::vec3       mPosition;
    float           mAspectRatio;
    float           mFieldOfView;
    float           mNearPlane;
    float           mFarPlane;
    Matrix          mPostViewTransform;
    Matrix          mSavedProjection;
    Matrix          mSavedModelView;
};