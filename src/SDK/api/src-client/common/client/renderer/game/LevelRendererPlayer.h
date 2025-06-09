#pragma once

#include "LevelRendererCamera.h"
#include "SDK/api/src/common/world/phys/Vec3.h"
#include "SDK/api/src-deps/Renderer/Camera.h"

class LevelRendererPlayer : public LevelRendererCamera {
public:
    SDK_API /*virtual*/ void setupCamera(mce::Camera &camera, const float a);

    SDK_API /*virtual*/ void queueRenderEntities(LevelRenderPreRenderUpdateParameters &levelRenderPreRenderUpdateParameters);
};