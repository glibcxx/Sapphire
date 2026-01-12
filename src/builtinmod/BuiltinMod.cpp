#include "BuiltinMod.h"

#include "SDK/api/sapphire/IMod.h"
#include "SDK/api/sapphire/hook/Hook.h"

#include "tickrate/TickRate.h"
#include "smoothpiston/SmoothPiston.h"
#include "chunkborder/ChunkBorderRenderer.h"
#include "freecamera/FreeCamera.h"
#include "BCM_V2/BCMMod.h"

namespace sapphire::example {

    SPHR_REGISTRY_MOD(BuiltinMod, instance)

    void BuiltinMod::onLoaded() {
        TickRateMod::getInstance();
        SmoothPistonMod::getInstance();
        FreeCameraMod::getInstance();
        installChunkBorderRender();
        BCM_V2::BCMMod::getInstance();
    }

    void BuiltinMod::onUnload() {
    }

    void BuiltinMod::onInit(const sapphire::ModInitContext &context) {
        mLog.debug("BuiltinMod inited!");
    }

    void BuiltinMod::onUnInit() {
        mLog.debug("BuiltinMod uninted!");
    }

    sapphire::Logger &BuiltinMod::getLogger() {
        return instance.mLog;
    }

} // namespace sapphire::example