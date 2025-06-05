#pragma once

#include "SDK/core/Core.h"

namespace rendergraph {
    class RenderContext;
}

class PlayerRenderView {
public:
    class LegacyPlayerRenderPass;

    class lambda_75d793f24d52434fc09af39da3e425e3 {
    public:
        SDK_API void operator()(const LegacyPlayerRenderPass &data, rendergraph::RenderContext &renderContext) const;
    };
};