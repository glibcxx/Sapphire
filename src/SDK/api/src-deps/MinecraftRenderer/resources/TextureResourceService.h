#pragma once

#include <variant>
#include "SDK/api/src-deps/Core/CheckedResourceService/ClientResourcePointer.h"
#include "SDK/api/src-external/RenderDragon/Resource/DragonTextureResourceService.h"

namespace mce {

    class Texture {
        std::byte _fill[104];
    };

    // size: 24
    class ClientTexture : public mce::ClientResourcePointer<std::variant<
                              std::monostate,
                              mce::Texture,
                              dragon::ClientTexture>> {};

} // namespace mce