#pragma once

#include "RenderMaterial.h"

namespace mce {

    class RenderMaterialGroupBase {
    public:
        virtual ~RenderMaterialGroupBase() = 0;

        virtual mce::RenderMaterialInfo &getMaterialInfo(const HashedString &name) = 0;

        virtual void clearMaterial(const HashedString &name) = 0;
    };

} // namespace mce