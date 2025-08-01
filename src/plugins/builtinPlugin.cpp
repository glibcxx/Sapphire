#include "builtinPlugin.h"

#include "SDK/api/sapphire/hook/Hook.h"

#include "tickrate/TickRateTest.h"
#include "smoothpiston/SmoothPiston.h"
#include "chunkborder/ChunkBorderRenderer.h"
#include "freecamera/FreeCamera.h"
#include "BCM/BCM.h"

void BuiltinPlugin::init() {
    TickRatePlugin::getInstance();
    SmoothPistonPlugin::getInstance();
    FreeCameraPlugin::getInstance();
    installChunkBorderRender();
    BedrockCameraMovementsPlugin::getInstance();
}

void BuiltinPlugin::uninit() {
}

static BuiltinPlugin instance{};