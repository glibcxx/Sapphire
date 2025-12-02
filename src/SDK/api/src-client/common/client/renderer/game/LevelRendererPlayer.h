#pragma once

#include "LevelRendererCamera.h"
#include "SDK/api/src/common/world/phys/Vec3.h"
#include "SDK/api/src-deps/Renderer/Camera.h"

class LevelRendererPlayer : public LevelRendererCamera {
public:
    SPHR_DECL_API("1.21.2,1.21.50,1.21.60", "call", "\xE8\x00\x00\x00\x00\xF3\x41\x0F\x10\x00\x08\x00\x8B\x00\x00\x03")
    SDK_API /*virtual*/ void setupCamera(mce::Camera &camera, const float a);

    SPHR_DECL_API("1.21.2", "\x40\x53\x57\x41\x57\x48\x83\xEC\x00\x48\x8B\xF9")
    SPHR_DECL_API("1.21.50,1.21.60", "\x40\x53\x55\x41\x55\x48\x83\xEC\x00\x48\x8B\x05")
    SDK_API /*virtual*/ void queueRenderEntities(LevelRenderPreRenderUpdateParameters &levelRenderPreRenderUpdateParameters) /*override*/;

    SPHR_DECL_API("1.21.2,1.21.50,1.21.60", "\x48\x8B\xC4\x48\x89\x58\x00\x55\x56\x57\x41\x54\x41\x55\x41\x56\x41\x57\x48\x81\xEC\x00\x00\x00\x00\x0F\x29\x70\x00\x0F\x29\x78\x00\x48\x8B\x05\x00\x00\x00\x00\x48\x33\xC4\x48\x89\x84\x24\x00\x00\x00\x00\x4C\x8B\xFA")
    SDK_API /*virtual*/ void callRenderCracks(BaseActorRenderContext &renderContext, const ViewRenderObject &renderObj) /*override*/;
};