#include "DrawUtils.h"

#include "SDK/api/sapphire/hook/Hook.h"
#include "SDK/api/src-client/common/client/renderer/MeshHelpers.h"
#include "SDK/api/src-deps/Coregraphics/RenderMaterialGroup.h"
#include "util/ScopeGuard.hpp"

#include "SDK/api/sapphire/event/EventManager.h"
#include "SDK/api/sapphire/event/events/RenderLevelEvent.h"

mce::MaterialPtr DrawUtils::sDrawMat{};

static DrawUtils *drawUtils = nullptr;

HOOK_TYPE(
    RenderLevelMainFuncHook,
    LevelRenderer,
    sapphire::hook::HookPriority::Normal,
    LevelRenderer::renderLevel,
    void,
    ScreenContext           &ctx,
    const FrameRenderObject &obj
) {
    drawUtils->mLevelRenderer = this;
    drawUtils->mScreenCtx = &ctx;
    drawUtils->mTess = &ctx.tessellator;
    this->origin(ctx, obj);
    sapphire::event::EventManager::getInstance().dispatchEvent(sapphire::event::RenderLevelEvent{this, ctx});

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

void DrawUtils::drawLine(const Vec3 &from, const Vec3 &to, const mce::Color &color) const {
    if (!this->mLevelRenderer) {
        Logger::Error("[DrawUtils] mLevelRenderer is nullptr");
        return;
    }
    Vec3           &camPos = this->mLevelRenderer->getLevelRendererPlayer().getCameraPos();
    std::lock_guard guard{this->mMutex};
    this->mTess->begin(mce::PrimitiveMode::LineList, 1, false);
    this->mTess->color(color);
    this->mTess->vertex(from - camPos);
    this->mTess->vertex(to - camPos);
}

void DrawUtils::drawQuard(const Vec3 &a, const Vec3 &b, const Vec3 &c, const Vec3 &d, const mce::Color &color) const {
    if (!this->mLevelRenderer) {
        Logger::Error("[DrawUtils] mLevelRenderer is nullptr");
        return;
    }
    Vec3           &camPos = this->mLevelRenderer->getLevelRendererPlayer().getCameraPos();
    std::lock_guard guard{this->mMutex};
    this->mTess->begin(mce::PrimitiveMode::LineList, 4, false);
    this->mTess->color(color);

    this->mTess->vertex(a - camPos);
    this->mTess->vertex(b - camPos);

    this->mTess->vertex(b - camPos);
    this->mTess->vertex(c - camPos);

    this->mTess->vertex(c - camPos);
    this->mTess->vertex(d - camPos);

    this->mTess->vertex(d - camPos);
    this->mTess->vertex(a - camPos);
}

// clang-format off
void DrawUtils::drawBox(const AABB &aabb, const mce::Color &color) const {
    if (!this->mLevelRenderer) {
        Logger::Error("[DrawUtils] mLevelRenderer is nullptr");
        return;
    }
    Vec3           &camPos = this->mLevelRenderer->getLevelRendererPlayer().getCameraPos();
    std::lock_guard guard{this->mMutex};

    std::array<Vec3, 8> c{
        Vec3{aabb.min.x, aabb.min.y, aabb.min.z},
        Vec3{aabb.max.x, aabb.min.y, aabb.min.z},
        Vec3{aabb.max.x, aabb.max.y, aabb.min.z},
        Vec3{aabb.min.x, aabb.max.y, aabb.min.z},
        Vec3{aabb.min.x, aabb.min.y, aabb.max.z},
        Vec3{aabb.max.x, aabb.min.y, aabb.max.z},
        Vec3{aabb.max.x, aabb.max.y, aabb.max.z},
        Vec3{aabb.min.x, aabb.max.y, aabb.max.z}
    };

    this->mTess->begin(mce::PrimitiveMode::LineList, 12, false);
    this->mTess->color(color);

    // Bottom face
    this->mTess->vertex(c[0] - camPos); this->mTess->vertex(c[1] - camPos);
    this->mTess->vertex(c[1] - camPos); this->mTess->vertex(c[5] - camPos);
    this->mTess->vertex(c[5] - camPos); this->mTess->vertex(c[4] - camPos);
    this->mTess->vertex(c[4] - camPos); this->mTess->vertex(c[0] - camPos);

    // Top face
    this->mTess->vertex(c[3] - camPos); this->mTess->vertex(c[2] - camPos);
    this->mTess->vertex(c[2] - camPos); this->mTess->vertex(c[6] - camPos);
    this->mTess->vertex(c[6] - camPos); this->mTess->vertex(c[7] - camPos);
    this->mTess->vertex(c[7] - camPos); this->mTess->vertex(c[3] - camPos);

    // Vertical edges
    this->mTess->vertex(c[0] - camPos); this->mTess->vertex(c[3] - camPos);
    this->mTess->vertex(c[1] - camPos); this->mTess->vertex(c[2] - camPos);
    this->mTess->vertex(c[4] - camPos); this->mTess->vertex(c[7] - camPos);
    this->mTess->vertex(c[5] - camPos); this->mTess->vertex(c[6] - camPos);
}
// clang-format on

void DrawUtils::flush() {
    if (!this->mLevelRenderer) {
        Logger::Error("[DrawUtils] mLevelRenderer is nullptr");
        return;
    }

    std::lock_guard guard{this->mMutex};
    if (!sDrawMat.mRenderMaterialInfoPtr && mce::RenderMaterialGroup::common) {
        mce::RenderMaterialInfo &matInfo = mce::RenderMaterialGroup::common->getMaterialInfo("wireframe");
        if (matInfo.mPtr)
            sDrawMat.mRenderMaterialInfoPtr = matInfo.shared_from_this();
        if (!sDrawMat.mRenderMaterialInfoPtr)
            Logger::Warn(
                "Material `wireframe` not found! mce::RenderMaterialGroup::common is {}",
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
