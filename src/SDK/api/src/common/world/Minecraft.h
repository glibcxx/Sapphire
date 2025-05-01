#pragma once

class GameCallbacks {
public:
    virtual ~GameCallbacks() = 0;

    virtual void onLevelCorrupt() = 0;

    virtual void onCriticalScriptError(const char *, const char *) = 0;

    virtual void onGameModeChanged() = 0;

    virtual void onBeforeSimTick() = 0;

    virtual void onTick(int a, int b) = 0;

    virtual void onInternetUpdate() = 0;

    virtual void onGameSessionReset() = 0;

    virtual void onLevelExit() = 0;

    virtual void onRequestResourceReload() = 0;

    virtual void updateScreens() = 0;
};

class Minecraft {
    virtual ~Minecraft() = 0;
};