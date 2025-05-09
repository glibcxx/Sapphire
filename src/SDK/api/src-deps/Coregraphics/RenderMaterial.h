#pragma once

#include <memory>
#include "RenderMaterialBase.h"
#include "SDK/api/src-deps/Core/Utility/StringHash.h"

namespace mce {

    class RenderMaterial : public cg::RenderMaterialBase {
    public:
        // ...
    };

    class RenderMaterialInfo : public std::enable_shared_from_this<RenderMaterialInfo> {
    public:
        HashedString                         mHashedName;
        std::unique_ptr<mce::RenderMaterial> mPtr;
    };
}