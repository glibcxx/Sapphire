#pragma once

#include <memory>
#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include "SDK/api/src-deps/Core/Utility/NonOwnerPointer.h"

class NetEventCallback;
class Minecraft;

class FreeCameraPlugin {
public:
    friend class CaptureClientMinecraftHook;

    static FreeCameraPlugin &getInstance();

    void enableFreeCamera(bool enable);

    bool isEnabled() {
        return this->mEnabled;
    }

    glm::vec3 mFreeCamPos;
    glm::quat mFreeCamOrientation;

    float mYaw = 0.0f;   // 偏航角
    float mPitch = 0.0f; // 俯仰角

    float mBaseCameraMoveSpeed = 10.0f;

    bool mRedirectMoveInputToFreeCamera = true;

private:
    bool mEnabled = false;

    Minecraft *mClientMinecraft;

    FreeCameraPlugin();
    ~FreeCameraPlugin();

    void _setupSettingGui();

    void onDrawSetting();
};