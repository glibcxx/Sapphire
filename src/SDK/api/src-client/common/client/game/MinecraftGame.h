#pragma once

#include "SDK/core/ApiManager.h"
#include "SDK/api/src/common/util/Timer.h"

class IClientInstance;

class MinecraftGame {
public:
    virtual ~MinecraftGame() = 0;

    SDK_API /*virtual*/ void update() /*override*/;
    MARK_HOOKABLE(&MinecraftGame::update)

    SDK_API void startFrame();
    MARK_HOOKABLE(&MinecraftGame::startFrame)

    SDK_API void endFrame();
    MARK_HOOKABLE(&MinecraftGame::endFrame)

    SDK_API void updateGraphics(IClientInstance &client, const Timer &timer);
    MARK_HOOKABLE(&MinecraftGame::updateGraphics)

    SDK_API void updateAudio(IClientInstance &client, const Timer &timer);
    MARK_HOOKABLE(&MinecraftGame::updateAudio)

    SDK_API void _update();
    MARK_HOOKABLE(&MinecraftGame::_update)

    SDK_API bool _clientUpdate();
    MARK_HOOKABLE(&MinecraftGame::_clientUpdate)
};