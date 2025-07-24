#pragma once

#include "SDK/api/src-client/common/client/renderer/rendergraph/PlayerRenderView.h"
#include "LevelRenderer.h"

class GameRenderer {
public:
    SDK_API std::weak_ptr<PlayerRenderView> addPlayerRenderView(LevelRenderer &renderer);
    MARK_HOOKABLE(&GameRenderer::addPlayerRenderView)
};