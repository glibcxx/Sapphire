#include "DrawUtils.h"

#include "SDK/api/src-client/common/client/renderer/MeshHelpers.h"
#include "SDK/api/src-deps/Coregraphics/RenderMaterialGroup.h"
#include "logger/GameLogger.hpp"
#include "util/ScopeGuard.hpp"

#include "SDK/api/sapphire/event/EventManager.h"
#include "SDK/api/sapphire/event/events/RenderLevelEvent.h"

#include "SDK/api/sapphire/hook/Hook.h"

mce::MaterialPtr DrawUtils::sDrawMat{};

static DrawUtils *drawUtils = nullptr;

HOOK_TYPE(
    RenderLevelMainFuncHook,
    LevelRenderer,
    hook::HookPriority::Normal,
    LevelRenderer::renderLevel,
    void,
    ScreenContext           &ctx,
    const FrameRenderObject &obj
) {
    drawUtils->mLevelRenderer = this;
    drawUtils->mScreenCtx = &ctx;
#if MC_VERSION == v1_21_2
    drawUtils->mTess = memory::getField<Tessellator *>(&ctx, 192);
#elif MC_VERSION == v1_21_50 || MC_VERSION == v1_21_60
    drawUtils->mTess = memory::getField<Tessellator *>(&ctx, 200);
#endif
    this->origin(ctx, obj);

    EventManager::getInstance().dispatchEvent(RenderLevelEvent{ctx});

    drawUtils->flush();
}

DrawUtils::DrawUtils(Tessellator *tess) :
    mTess(tess) {
    if (!RenderLevelMainFuncHook::hook())
        Logger::Error("[DrawUtils] RenderLevelMainFuncHook::hook failed!");
    else
        Logger::Debug("[DrawUtils] initialized!");
}

DrawUtils::~DrawUtils() {
    RenderLevelMainFuncHook::unhook();
}

void DrawUtils::drawLine(const Vec3 &from, const Vec3 &to, const mce::Color &color) {
    if (!this->mLevelRenderer) {
        Logger::Error("[DrawUtils] mLevelRenderer is nullptr");
        return;
    }
    std::lock_guard guard{this->mMutex};
    this->mTess->begin(mce::PrimitiveMode::Lines, 1, false);
    Vec3 &camPos = this->mLevelRenderer->getLevelRendererPlayer().getCameraPosition();
    this->mTess->color(color);
    this->mTess->vertex(from - camPos);
    this->mTess->vertex(to - camPos);
}

void DrawUtils::flush() {
    if (!this->mLevelRenderer) {
        Logger::Error("[DrawUtils] mLevelRenderer is nullptr");
        return;
    }

    std::lock_guard guard{this->mMutex};
    if (!sDrawMat.mRenderMaterialInfoPtr && mce::RenderMaterialGroup::common) {
        mce::RenderMaterialInfo &matInfo = mce::RenderMaterialGroup::common->getMaterialInfo("selection_box");
        if (matInfo.mPtr)
            sDrawMat.mRenderMaterialInfoPtr = matInfo.shared_from_this();
        if (!sDrawMat.mRenderMaterialInfoPtr)
            Logger::Warn(
                "Material `selection_box` not found! mce::RenderMaterialGroup::common is {}",
                (void *)mce::RenderMaterialGroup::common
            );
    }

    char a4[64] = {};
    MeshHelpers::renderMeshImmediately(*this->mScreenCtx, *this->mTess, sDrawMat, a4);
}

DrawUtils &DrawUtils::getInstance() {
    static DrawUtils d{};
    drawUtils = &d;
    return d;
}
