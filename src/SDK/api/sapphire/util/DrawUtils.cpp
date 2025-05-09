#include "DrawUtils.h"

#include "hook/Hook.hpp"
#include "SDK/api/src-client/common/client/renderer/MeshHelpers.h"
#include "SDK/api/src-deps/Coregraphics/RenderMaterialGroup.h"
#include "logger/GameLogger.hpp"
#include "util/ScopeGuard.hpp"

mce::MaterialPtr DrawUtils::sDrawMat{};

static DrawUtils *drawUtils = nullptr;

HOOK_TYPE(GetScreenCtx, LevelRenderer, LevelRenderer::renderLevel, void, ScreenContext &ctx, const FrameRenderObject &obj) {
    drawUtils->mLevelRenderer = this;
    drawUtils->mScreenCtx = &ctx;
#if MC_VERSION == v1_21_2
    drawUtils->mTess = memory::getField<Tessellator *>(&ctx, 192);
#elif MC_VERSION == v1_21_50 || MC_VERSION == v1_21_60
    drawUtils->mTess = memory::getField<Tessellator *>(&ctx, 200);
#endif
    this->origin(ctx, obj);

    // drawUtils->drawLine({0.0f, 0.0f, 0.0f}, {10.0f, 10.0f, 10.0f});
    // drawUtils->flush();
}

DrawUtils::DrawUtils(Tessellator *tess) :
    mTess(tess) {
    GetScreenCtx::hook();
}

DrawUtils::~DrawUtils() {
    GetScreenCtx::unhook();
}

void DrawUtils::drawLine(const Vec3 &from, const Vec3 &to, const mce::Color &color) {
    this->mTess->begin(mce::PrimitiveMode::Lines, 1, false);
    Vec3 &camPos = this->mLevelRenderer->getLevelRendererPlayer().getCameraPosition();
    this->mTess->color(color);
    this->mTess->vertex(from - camPos);
    this->mTess->vertex(to - camPos);
}

void DrawUtils::flush() {
    if (!sDrawMat.mRenderMaterialInfoPtr && mce::RenderMaterialGroup::common) {
        mce::RenderMaterialInfo &matInfo = mce::RenderMaterialGroup::common->getMaterialInfo("selection_box");
        if (matInfo.mPtr)
            sDrawMat.mRenderMaterialInfoPtr = matInfo.shared_from_this();
        if (!sDrawMat.mRenderMaterialInfoPtr)
            Logger::Warn("Material `selection_box` not found! mce::RenderMaterialGroup::common is {}", (void *)mce::RenderMaterialGroup::common);
    }

    char a4[64] = {};
    MeshHelpers::renderMeshImmediately(*this->mScreenCtx, *this->mTess, sDrawMat, a4);
}

DrawUtils &DrawUtils::getInstance() {
    static DrawUtils d{};
    drawUtils = &d;
    return d;
}
