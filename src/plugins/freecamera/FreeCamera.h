#pragma once

#include <memory>
#include <glm/glm.hpp>
#include <glm/ext.hpp>

class FreeCameraPlugin {
public:
    static FreeCameraPlugin &getInstance();

    glm::vec3 mFreeCamPos;
    glm::quat mFreeCamOrientation;

    float mYaw = 0.0f;   // 偏航角
    float mPitch = 0.0f; // 俯仰角

    bool mEnabled = false;

private:
    FreeCameraPlugin();
    ~FreeCameraPlugin();

    void _setupSettingGui();

    void onDrawSetting();
};