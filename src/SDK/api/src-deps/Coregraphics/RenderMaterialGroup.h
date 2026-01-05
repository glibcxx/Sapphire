#pragma once

#include "SDK/api/src/common/AppPlatformListener.h"
#include "RenderMaterialGroupBase.h"

namespace mce {

    class RenderMaterialGroup : public AppPlatformListener, public RenderMaterialGroupBase {
    public:
        // vtb+1
        virtual mce::RenderMaterialInfo &getMaterialInfo(const HashedString &name) override;

        // vtb+2
        virtual void clearMaterial(const HashedString &name) override;

        SPHR_DECL_API("1.21.2,1.21.50,1.21.60", "lea", "\x48\x8D\x0D\x00\x00\x00\x00\xE8\x00\x00\x00\x00\xF3\x0F\x10\x0D")
        SDK_API static RenderMaterialGroup common;

        SPHR_DECL_API("1.21.2,1.21.50,1.21.60", "lea", "\x48\x8D\x05\x00\x00\x00\x00\x48\x89\x41\x00\x0F\x57\xC0\x0F\x11\x41\x00\x48\x89\x79")
        SDK_API static RenderMaterialGroup switchable;
    };

} // namespace mce