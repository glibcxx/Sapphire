#pragma once

#include <optional>
#include "../Rendering/BufferHandleType.h"

namespace dragon::mesh {

    template <typename Ty>
    struct RegistryTraits {
        using TBufferHandle = std::optional<dragon::rendering::details::BufferHandleTypeWrapper<typename Ty::HandleTypeKey>>;
        using THandle = TBufferHandle;
        using TContext = typename Ty::Context;
        using TKeyType = typename Ty::KeyType;
    };

} // namespace dragon::mesh