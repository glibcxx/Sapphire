#include "builtinPlugin.h"

#include "SDK/api/sapphire/hook/Hook.h"

#include "tickrate/TickRateTest.h"
#include "smoothpiston/SmoothPiston.h"
#include "chunkborder/ChunkBorderRenderer.h"
#include "freecamera/FreeCamera.h"
#include "BCM_V2/BCMPlugin.h"

void BuiltinPlugin::onLoaded() {
    TickRatePlugin::getInstance();
    SmoothPistonPlugin::getInstance();
    FreeCameraPlugin::getInstance();
    installChunkBorderRender();
    BCM_V2::BCMPlugin::getInstance();
}

void BuiltinPlugin::onUnload() {
}

void BuiltinPlugin::onInit() {
    Logger::Debug("[BuiltinPlugin] onInit!");
}

void BuiltinPlugin::onUnInit() {
    Logger::Debug("[BuiltinPlugin] onUnInit!");
}

static BuiltinPlugin instance{};