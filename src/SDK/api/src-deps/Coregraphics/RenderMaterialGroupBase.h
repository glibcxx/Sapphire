#pragma once

#include "RenderMaterial.h"

namespace mce {

    // size: 8
    class RenderMaterialGroupBase {
    public:
        using MaterialMap = std::unordered_map<HashedString,std::shared_ptr<mce::RenderMaterialInfo>> ;

        // vtb+0
        virtual ~RenderMaterialGroupBase() = default;

        // vtb+1
        virtual mce::RenderMaterialInfo &getMaterialInfo(const HashedString &name) = 0;

        // vtb+2
        virtual void clearMaterial(const HashedString &name) = 0;
    };

} // namespace mce