#include "FreeCamera.h"

#include "SDK/api/sapphire/hook/Hook.h"
#include "SDK/api/sapphire/GUI/GUI.h"
#include "SDK/api/sapphire/service/Service.h"
#include "SDK/api/src-client/common/client/game/ClientInstance.h"
#include "SDK/api/src-client/common/client/renderer/game/LevelRendererPlayer.h"
#include "SDK/api/src-client/common/client/player/LocalPlayer.h"
#include "SDK/api/src/common/entity/systems/ClientInputUpdateSystem.h"

#include "SDK/api/src/common/network/packet/TextPacket.h"
#include "SDK/api/src/common/world/Minecraft.h"
#include "SDK/api/src/common/world/GameSession.h"

FreeCameraPlugin *freeCam = nullptr;

HOOK_TYPE(
    CaptureClientMinecraftHook,
    Minecraft,
    sapphire::hook::HookPriority::Normal,
    Minecraft::startClientGame,
    void,
    std::unique_ptr<NetEventCallback> legacyClientNetworkHandler
) {
    freeCam->mClientMinecraft = this;
    this->origin(std::move(legacyClientNetworkHandler));
}

HOOK_TYPE_CONST(
    ForceDrawPlayerHook,
    ClientInstance,
    sapphire::hook::HookPriority::Normal,
    ClientInstance::getRenderPlayerModel,
    bool
) {
    return (freeCam->isEnabled() && freeCam->mShowPlayerModel) || this->origin();
}

HOOK_TYPE(
    ClientInputUpdateSystemTickHook,
    ClientInputUpdateSystemInternal,
    sapphire::hook::HookPriority::Normal,
    ClientInputUpdateSystemInternal::tick,
    void,
    void *context
) {
    if (freeCam->isEnabled()) {
        return;
    }
    this->origin(context);
}

HOOK_TYPE(
    OnPlayerTurnHook,
    LocalPlayer,
    sapphire::hook::HookPriority::Normal,
    LocalPlayer::localPlayerTurn,
    void,
    const Vec2 &deltaRot
) {
    if (freeCam->isEnabled() && freeCam->mRedirectMoveInputToFreeCamera) {
        freeCam->rotateFreeCamDelta(deltaRot);

        auto *moveInput = sapphire::getLocalPlayer()->getMoveInputComponent();
        if (moveInput) {
            float forward = moveInput->mRawInputState.mUp - moveInput->mRawInputState.mDown;
            float right = moveInput->mRawInputState.mRight - moveInput->mRawInputState.mLeft;
            float up = moveInput->mRawInputState.mJumpDown - moveInput->mRawInputState.mSneakDown;
            float baseMoveSpeed = freeCam->mBaseCameraMoveSpeed;
            auto &io = ImGui::GetIO();
            GuiOverlay::getInputManager().disableGameMouseWheelInput(moveInput->mRawInputState.mSprintDown);
            if (moveInput->mRawInputState.mSprintDown) {
                freeCam->mBaseCameraMoveSpeed += io.MouseWheel;
                freeCam->mBaseCameraMoveSpeed = std::clamp(freeCam->mBaseCameraMoveSpeed, 1.0f, 50.0f);
                baseMoveSpeed *= 5.0f;
            }

            float deltaTime = ImGui::GetIO().DeltaTime;
            float moveSpeed = baseMoveSpeed * deltaTime;
            if (right == 0 && forward == 0) {
                freeCam->mFreeCamPos += glm::vec3(0, up * moveSpeed, 0);
            } else {
                glm::vec3 camera3DForward = freeCam->getFreeCamOrientation() * glm::vec3(right, 0.0f, -forward);

                glm::vec3 horiNorm = glm::normalize(glm::vec3(camera3DForward.x, 0.0f, camera3DForward.z));
                glm::vec3 upDir = glm::vec3(0, up, 0);
                freeCam->mFreeCamPos += (horiNorm + upDir) * moveSpeed;
            }
        }
        return;
    }
    return this->origin(deltaRot);
}

HOOK_TYPE(
    SetCameraPosHook,
    LevelRendererPlayer,
    sapphire::hook::HookPriority::Normal,
    LevelRendererPlayer::setupCamera,
    void,
    mce::Camera &camera,
    const float  a
) {
    if (freeCam->isEnabled()) {
        auto *camComp = ClientInstance::primaryClientInstance->getRenderCameraComponent();
        if (camComp) {
            camComp->mCamera.mPosition = freeCam->mFreeCamPos;
            camComp->mCamera.mOrientation = freeCam->getFreeCamOrientation();
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
    if (CaptureClientMinecraftHook::hook())
        Logger::Debug("[FreeCameraPlugin] CaptureClientMinecraftHook initialized!");
    else
        Logger::Error("[FreeCameraPlugin] CaptureClientMinecraftHook::hook failed!");
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
    if (!ClientInputUpdateSystemTickHook::hook())
        Logger::Error("[FreeCameraPlugin] ClientInputUpdateSystemTickHook::hook failed!");
    else
        Logger::Debug("[FreeCameraPlugin] ClientInputUpdateSystemTickHook initialized!");
}

FreeCameraPlugin::~FreeCameraPlugin() {
    ClientInputUpdateSystemTickHook::unhook();
    OnPlayerTurnHook::unhook();
    SetCameraPosHook::unhook();
    ForceDrawPlayerHook::unhook();
    CaptureClientMinecraftHook::unhook();
}

void FreeCameraPlugin::setFreeCamOrientation(const glm::quat &orientation) {
    this->mFreeCamOrientation = orientation;
    glm::vec3 forward = freeCam->mFreeCamOrientation * glm::vec3(0.0f, 0.0f, -1.0f);
    mPitch = glm::degrees(std::asin(glm::clamp(forward.y, -1.0f, 1.0f)));
    glm::quat pitchQuat = glm::angleAxis(glm::radians(mPitch), glm::vec3(1, 0, 0));
    glm::quat yawQuatOnly = freeCam->mFreeCamOrientation * glm::inverse(pitchQuat);
    float     angleDeg = glm::degrees(glm::angle(yawQuatOnly));
    glm::vec3 axis = glm::axis(yawQuatOnly);
    mYaw = (axis.y >= 0.0f ? 1.0f : -1.0f) * angleDeg;
}

void FreeCameraPlugin::rotateFreeCamDelta(const Vec2 &deltaRot) {
    this->mYaw -= deltaRot.y;
    this->mPitch += deltaRot.x;
    this->mPitch = std::clamp(this->mPitch, -90.0f, 90.0f);
    glm::quat yawQuat = glm::angleAxis(glm::radians(this->mYaw), glm::vec3(0.0f, 1.0f, 0.0f));
    glm::quat pitchQuat = glm::angleAxis(glm::radians(this->mPitch), glm::vec3(1.0f, 0.0f, 0.0f));
    this->mFreeCamOrientation = yawQuat * pitchQuat;
}

void FreeCameraPlugin::enableFreeCamera(bool enable, bool showPlayerModel) {
    this->mEnabled = enable;
    this->mShowPlayerModel = showPlayerModel;
    if (this->mEnabled) {
        auto &cam = ClientInstance::primaryClientInstance->getRenderCameraComponent()->mCamera;
        this->mFreeCamPos = cam.mPosition;
        this->setFreeCamOrientation(cam.mOrientation);
    }
    if (this->mClientMinecraft) {
        TextPacket packet = TextPacket::createRaw(std::format("FreeCamera: {}", this->mEnabled ? "ON" : "OFF"));
        packet.mType = TextPacketType::Tip;
        this->mClientMinecraft->mGameSession->mLegacyClientNetworkHandler->handle({}, packet);
    }
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
             if (ClientInstance::primaryClientInstance->isShowingMenu())
                 return;
             this->enableFreeCamera(!this->mEnabled);
         }}
    );
}

void FreeCameraPlugin::onDrawSetting() {
    if (ImGui::Checkbox("Toggle Free Camera", &mEnabled) && this->mEnabled) {
        this->enableFreeCamera(true);
    }
}
