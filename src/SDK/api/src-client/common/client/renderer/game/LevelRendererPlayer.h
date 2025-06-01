#pragma once

#include "SDK/api/src/common/world/phys/Vec3.h"
#include "LevelRendererCamera.h"

namespace mce {
    class Camera;
}

class LevelRendererPlayer : public LevelRendererCamera {
public:
    /*virtual*/ SDK_API void setupCamera(mce::Camera &camera, const float a);
};