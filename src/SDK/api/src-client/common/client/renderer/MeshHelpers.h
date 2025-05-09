#pragma once

#include "SDK/api/src-client/common/client/gui/screen/ScreenContext.h"
#include "SDK/api/src-client/common/client/renderer/Tessellator.h"
#include "SDK/api/src-client/common/client/renderer/MaterialPtr.h"

namespace MeshHelpers {

    SDK_API void renderMeshImmediately(
        ScreenContext &screenContext, Tessellator &tessellator, const mce::MaterialPtr &material, void *a4
    );

};
