#include "FreeCamera.h"

#include "SDK/api/sapphire/GUI/GUI.h"
#include "SDK/api/sapphire/service/Service.h"
#include "SDK/api/src-client/common/client/game/ClientInstance.h"
#include "SDK/api/src-client/common/client/renderer/game/LevelRendererPlayer.h"
#include "SDK/api/src-client/common/client/player/LocalPlayer.h"

FreeCameraPlugin *freeCam = nullptr;

HOOK_TYPE_CONST(
    ForceDrawPlayerHook,
    ClientInstance,
    hook::HookPriority::Normal,
    ClientInstance::getRenderPlayerModel,
    bool
) {
    return freeCam->mEnabled || this->origin();
}

HOOK_TYPE(
    OnPlayerTurnHook,
    LocalPlayer,
    hook::HookPriority::Normal,
    LocalPlayer::localPlayerTurn,
    void,
    const Vec2 &deltaRot
) {
    if (freeCam->mEnabled) {
        freeCam->mYaw -= deltaRot.y;
        freeCam->mPitch += deltaRot.x;
        freeCam->mPitch = std::clamp(freeCam->mPitch, -90.0f, 90.0f);

        glm::quat yawQuat = glm::angleAxis(glm::radians(freeCam->mYaw), glm::vec3(0.0f, 1.0f, 0.0f));
        glm::quat pitchQuat = glm::angleAxis(glm::radians(freeCam->mPitch), glm::vec3(1.0f, 0.0f, 0.0f));
        freeCam->mFreeCamOrientation = yawQuat * pitchQuat;
        return;
    }
    return this->origin(deltaRot);
}

HOOK_TYPE(
    SetCameraPosHook,
    LevelRendererPlayer,
    hook::HookPriority::Normal,
    LevelRendererPlayer::setupCamera,
    void,
    mce::Camera &camera,
    const float  a
) {
    if (freeCam->mEnabled) {
        float forward = ImGui::IsKeyDown(ImGuiKey_W) - ImGui::IsKeyDown(ImGuiKey_S);
        float right = ImGui::IsKeyDown(ImGuiKey_D) - ImGui::IsKeyDown(ImGuiKey_A);
        float up = ImGui::IsKeyDown(ImGuiKey_Space) - ImGui::IsKeyDown(ImGuiKey_ModShift);
        auto *camComp = ClientInstance::primaryClientInstance->getRenderCameraComponent();

        if (camComp) {
            float baseMoveSpeed = 5.0f;
            if (ImGui::IsKeyDown(ImGuiKey_ModCtrl)) {
                baseMoveSpeed *= 5.0f;
            }

            float deltaTime = ImGui::GetIO().DeltaTime;
            float moveSpeed = baseMoveSpeed * deltaTime;
            if (right == 0 && forward == 0) {
                freeCam->mFreeCamPos += glm::vec3(0, up * moveSpeed, 0);
            } else {
                glm::vec3 camera3DForward = freeCam->mFreeCamOrientation * glm::vec3(right, 0.0f, -forward);

                glm::vec3 horiNorm = glm::normalize(glm::vec3(camera3DForward.x, 0.0f, camera3DForward.z));
                glm::vec3 upDir = glm::vec3(0, up, 0);
                freeCam->mFreeCamPos += (horiNorm + upDir) * moveSpeed;
            }
            camComp->mCamera.mPosition = freeCam->mFreeCamPos;
            camComp->mCamera.mOrientation = freeCam->mFreeCamOrientation;
        }
    }
    this->origin(camera, a);
}

FreeCameraPlugin &FreeCameraPlugin::getInstance() {
    static FreeCameraPlugin instace{};
    return instace;
}

FreeCameraPlugin::FreeCameraPlugin() {
    _setupSettingGui();
    freeCam = this;
    if (ForceDrawPlayerHook::hook())
        Logger::Debug("[FreeCameraPlugin] ForceDrawPlayerHook initialized!");
    else
        Logger::Error("[FreeCameraPlugin] ForceDrawPlayerHook::hook failed!");
    if (SetCameraPosHook::hook())
        Logger::Debug("[FreeCameraPlugin] SetCameraPosHook initialized!");
    else
        Logger::Error("[FreeCameraPlugin] SetCameraPosHook::hook failed!");
    if (OnPlayerTurnHook::hook())
        Logger::Debug("[FreeCameraPlugin] OnPlayerTurnHook initialized!");
    else
        Logger::Error("[FreeCameraPlugin] OnPlayerTurnHook::hook failed!");
}

FreeCameraPlugin::~FreeCameraPlugin() {
    ForceDrawPlayerHook::unhook();
}

void FreeCameraPlugin::_setupSettingGui() {
    GuiOverlay::registerPluginSettings(
        {.name = "Free Camera",
         .description = "Free Camera",
         .drawSettings = [this]() { this->onDrawSetting(); }}
    );

    GuiOverlay::registerHotkey(
        {.triggerKey = ImGuiKey_O,
         .description = "Toggle Free Camera",
         .action = [this]() {
             Level *level = sapphire::getMultiPlayerLevel();
             this->mEnabled = !this->mEnabled;
             GuiOverlay::getInputManager().disableGameKeyBoardInput(this->mEnabled);
             GuiOverlay::getInputManager().disableGameMouseWheelInput(this->mEnabled);
             if (this->mEnabled) {
                 auto &cam = ClientInstance::primaryClientInstance->getRenderCameraComponent()->mCamera;
                 this->mFreeCamPos = cam.mPosition;
                 this->mFreeCamOrientation = cam.mOrientation;
                 glm::vec3 eulerAngles = glm::eulerAngles(this->mFreeCamOrientation);
                 glm::vec3 forward = freeCam->mFreeCamOrientation * glm::vec3(0.0f, 0.0f, -1.0f);
                 mPitch = glm::degrees(std::asin(glm::clamp(forward.y, -1.0f, 1.0f)));
                 glm::quat pitchQuat = glm::angleAxis(glm::radians(mPitch), glm::vec3(1, 0, 0));
                 glm::quat yawQuatOnly = freeCam->mFreeCamOrientation * glm::inverse(pitchQuat);
                 float     angleDeg = glm::degrees(glm::angle(yawQuatOnly));
                 glm::vec3 axis = glm::axis(yawQuatOnly);
                 mYaw = (axis.y >= 0.0f ? 1.0f : -1.0f) * angleDeg;
             }
         }}
    );
}

void FreeCameraPlugin::onDrawSetting() {
    ImGui::Checkbox("Toggle Free Camera", &mEnabled);
}
