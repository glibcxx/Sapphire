#pragma once

#include <glm/glm.hpp>
#include "SDK/api/src-client/common/client/renderer/culling/FrustumCuller.h"

class BlockSource;
enum class LevelCullerType : uint8_t;
enum class FrustumCullerType : uint8_t;

enum class FakeHDRSetting : int32_t {
    EnableFakeHDR = 0,
    DisableFakeHDR = 1,
}; // FakeHDRSetting

// size: 184
struct LevelRenderPreRenderUpdateParameters {
    FrustumCuller &leftCuller;  // off+0
    FrustumCuller &rightCuller; // off+8

    glm::tvec3<float> cameraWorldPosition;       // off+16
    glm::tvec3<float> cameraTargetWorldPosition; // off+28
    glm::tvec3<float> cameraForward;             // off+40

    uint32_t       mUnk52[6];       // off+52
    const uint32_t viewBlockRadius; // off+76
    const float    currentTime;     // off+80
    BlockSource   *region;          // off+88
    bool           stereoRendering; // off+96
    FakeHDRSetting enableFakeHDR;   // off+100

    float skyIntensityOverride; // off+104
    float renderDistanceScalar; // off+108

    LevelCullerType   levelCullerType;   // off+112
    FrustumCullerType frustumCullerType; // off+113
    uint8_t           mUnk114;           // off+114

    std::function<void(LevelRenderPreRenderUpdateParameters &)> preLevelBuildCallback; // off+120
};
static_assert(sizeof(LevelRenderPreRenderUpdateParameters) == 184);