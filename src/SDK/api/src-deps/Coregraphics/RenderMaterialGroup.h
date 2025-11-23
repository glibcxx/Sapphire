#pragma once

#include "SDK/core/SymbolResolver.h"
#include "RenderMaterialGroupBase.h"

namespace mce {

    class RenderMaterialGroup : public RenderMaterialGroupBase {
    public:
        SDK_API static RenderMaterialGroup *common;
        SDK_API static RenderMaterialGroup *switchable;
    };

} // namespace mce