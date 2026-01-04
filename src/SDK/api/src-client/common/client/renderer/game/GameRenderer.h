#pragma once

#include "SDK/api/src-client/common/client/renderer/rendergraph/PlayerRenderView.h"
#include "LevelRenderer.h"

class GameRenderer {
public:
    SPHR_DECL_API("1.21.2", "\x48\x89\x5C\x24\x00\x48\x89\x6C\x24\x00\x48\x89\x74\x24\x00\x57\x48\x83\xEC\x00\x48\x8B\xFA\x48\x8B\xD9\x33\xED")
    SPHR_DECL_API("1.21.50,1.21.60", "\x48\x89\x5C\x24\x00\x48\x89\x6C\x24\x00\x56\x57\x41\x56\x48\x83\xEC\x00\x48\x8B\xF2\x48\x8B\xF9\x48\x89\x54\x24\x00\x00\x8B")
    SDK_API std::weak_ptr<PlayerRenderView> addPlayerRenderView(LevelRenderer &renderer);

    SPHR_DECL_API("1.21.2", "call", "\xE8\x00\x00\x00\x00\x4C\x8B\xE0\x80\xBE")
    SPHR_DECL_API("1.21.50,1.21.60", "call", "\xE8\x00\x00\x00\x00\x48\x8B\xF8\x41\x80\xBE")
    SDK_API ScreenContext makeScreenContext(const float a);

    SPHR_DECL_API("1.21.2,1.21.50,1.21.60", "disp:14,call", "\x90\xF3\x41\x0F\x10\x4D\x08")
    SDK_API void renderCurrentFrame(float a);

    SPHR_DECL_API("1.21.2", "call", "\xE8\x00\x00\x00\x00\x4C\x8B\xE8\x4C\x3B\xE0")
    SPHR_DECL_API("1.21.50,1.21.60", "call", "\xE8\x00\x00\x00\x00\x48\x8B\xF8\x4C\x8D\x6E")
    SDK_API std::unique_ptr<FrameRenderObject, std::function<void(FrameRenderObject *)>> _extractFrame(
        ScreenContext &screenContext, bool renderGraphContainsPlayScreen
    );
};