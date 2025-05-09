#pragma once

#include "SDK/core/Core.h"
#include "RenderMaterialGroupBase.h"

namespace mce {

    class RenderMaterialGroup : public RenderMaterialGroupBase {
    public:
        SDK_API static RenderMaterialGroup *common;
    };

} // namespace mce