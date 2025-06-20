#pragma once

#include "SDK/api/src-deps/Core/CheckedResourceService/ServerResourcePointer.h"

namespace dragon::res {

    class ResolvedTextureResource;
    class TextureDescription;

    class ServerTexture : public mce::ServerResourcePointer<ResolvedTextureResource> {
    };

} // namespace dragon::res
