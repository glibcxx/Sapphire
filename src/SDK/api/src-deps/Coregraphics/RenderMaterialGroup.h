#pragma once

#include "SDK/core/ApiManager.h"
#include "RenderMaterialGroupBase.h"

namespace mce {

    class RenderMaterialGroup : public RenderMaterialGroupBase {
    public:
        SDK_API static RenderMaterialGroup      *common;
        SDK_API static mce::RenderMaterialGroup *switchable;
    };

} // namespace mce