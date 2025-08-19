#pragma once

#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include "SDK/api/src-deps/Core/Utility/NonOwnerPointer.h"
#include "SDK/api/src/common/world/phys/Vec2.h"

class NetEventCallback;
class Minecraft;

class FreeCameraPlugin {
public:
    friend class CaptureClientMinecraftHook;

    static FreeCameraPlugin &getInstance();

    void enableFreeCamera(bool enable, bool showPlayerModel = true);

    bool isEnabled() {
        return this->mEnabled;
    }

    void setFreeCamOrientation(const glm::quat &orientation);
    void rotateFreeCamDelta(const Vec2 &deltaRot);

    const glm::quat &getFreeCamOrientation() {
        return this->mFreeCamOrientation;
    }

    glm::vec3 getForwardVector() const {
        return this->mFreeCamOrientation * glm::vec3(0.0f, 0.0f, -1.0f);
    }

    float getFreeCamYaw() { return this->mYaw; }
    float getFreeCamPitch() { return this->mPitch; }

    glm::vec3 mFreeCamPos;

private:
    glm::quat mFreeCamOrientation;

    float mYaw = 0.0f;   // 偏航角
    float mPitch = 0.0f; // 俯仰角

public:
    float mBaseCameraMoveSpeed = 10.0f;

    bool mRedirectMoveInputToFreeCamera = true;

    bool mShowPlayerModel = true;

private:
    bool mEnabled = false;

    Minecraft *mClientMinecraft = nullptr;

    FreeCameraPlugin();
    ~FreeCameraPlugin();

    void _setupSettingGui();

    void onDrawSetting();

    Minecraft *getMinecraft();
};