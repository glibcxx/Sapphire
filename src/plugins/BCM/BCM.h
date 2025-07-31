#pragma once

#include "../freecamera/FreeCamera.h"
#include "editor/BCMEditor.h"
#include "SDK/api/sapphire/event/events/gui/GuiOverlayFrameEvent.h"

class BedrockCameraMovementsPlugin {
public:
    FreeCameraPlugin &mFreeCam;

private:
    std::unique_ptr<BCMEditor> mEditor;
    bool                       mInEditMode = false;

public:
    static BedrockCameraMovementsPlugin &getInstance();

    void addKeyFrameAtCurrentCameraPos();

    void setEditMode(bool enable);

    void setPlaying(bool playing);

    // 播放或拖动时间轴光标时，更新相机状态
    void update(float alpha);

    void setCameraPos(CameraState &cam);

    void renderHUD(sapphire::event::GuiOverlayFrameEvent &e);

private:
    BedrockCameraMovementsPlugin();
    ~BedrockCameraMovementsPlugin();

    void _setupSettingMenu();

    void _onRenderSetting();
};