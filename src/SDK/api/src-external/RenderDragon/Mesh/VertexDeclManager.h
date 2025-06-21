#pragma once

#include <optional>
#include <shared_mutex>

#include "SDK/core/Core.h"
#include "SDK/api/src-external/RenderDragon/Endstone/Guarded.hpp"
#include "SDK/api/src-deps/Core/Threading/InstancedThreadLocal.h"

namespace dragon::mesh {

    // size: 8
    class VertexDeclManager {
    public:
        // size: 312
        class Impl {
        public:
            class DeclLayout {
            public:
                // bgfx::VertexDeclHandle mHandle;
                // bgfx::VertexDecl       mDecl;
            };
            class ValidationKey {
            public:
                uint32_t mMaterial;
                uint64_t mPass;
                uint32_t mVertexDecl;
            };

            dragon::guarded::Guarded<
                std::unordered_map<uint32_t, DeclLayout>,
                std::shared_mutex>
                mHashTable; // off+0

            dragon::guarded::Guarded<
                std::unordered_map<uint16_t, const DeclLayout *>,
                std::shared_mutex>
                mHandleTable; // off+72

            Bedrock::Threading::InstancedThreadLocal<
                int /* boost::container::flat_map<ValidationKey, bool> */>
                mValidationCache; // off+144

            SDK_API Impl *ctor();
        };

        std::unique_ptr<Impl> mImpl; // off+0
    };
    static_assert(sizeof(VertexDeclManager::Impl) == 312);

} // namespace dragon::mesh