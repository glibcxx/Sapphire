#pragma once

#include "Resource.h"
#include "SDK/api/src-deps/Core/Math/Color.h"

namespace mce {
    class RenderContext;
    class FrameBufferObject;
    class Texture;
    class TextureDescription;
} // namespace mce

namespace rendergraph {

    class Pass;

    // size: 176
    class APIResourcePool {
    public:
        mce::RenderContext                                                       &mRawRenderContext;        // off+0
        std::unordered_map<const Pass *, std::unique_ptr<mce::FrameBufferObject>> mLinkedFrameBuffers;      // off+8
        std::vector<std::unique_ptr<mce::Texture>>                                mTransientTexturePool;    // off+72
        std::map<mce::TextureDescription, std::vector<mce::Texture *>>            mFreeTransientTextures;   // off+96
        std::unordered_map<const Resource *, mce::Texture *>                      mLinkedTransientTextures; // off+112
    };

    // size: 8
    class Pass {
    public:
        // vtb+0
        virtual ~Pass() = default;

        // vtb+1
        virtual void execute(RenderContext &renderCtx) = 0;
    };

    // size: 8
    class RenderPass : public Pass {
    public:
        // vtb+2
        virtual void render(RenderContext &renderCtx) = 0;
    };

    // size: 72 + sizeof(T)
    template <typename T>
    class CustomRenderPass : RenderPass {
    public:
        std::function<void(const T &, RenderContext &)> mCallback; // off+8
        T                                               mData;     // off+72
    };

    // size: 72
    template <>
    class CustomRenderPass<void> : RenderPass {
    public:
        std::function<void(RenderContext &)> mCallback; // off+8
    };
    static_assert(sizeof(CustomRenderPass<void>) == 72);

    class RenderOrder {
    public:
        // size: 48
        struct PassStorage {
            std::vector<std::shared_ptr<Pass>> mPasses;       // off+0
            std::vector<Pass *>                mLinkedPasses; // off+24
        };

        // size: 88
        struct ResourceStorage {
            std::vector<std::shared_ptr<Resource>>      mResources;      // off+0
            std::unordered_map<std::string, Resource *> mNamedResources; // off+24
        };

        // size: 64
        struct PassRegistry {
            enum class FrameBufferRequirement : int32_t {
                UNSPECIFIED = 0,
                FORBIDDEN = 1,
                REQUIRED = 2,
            };

            enum class ClearTarget : uint8_t {
                None = 0,
                Color = 1,
                Depth = 2,
                Stencil = 4,
            }; // ClearTarget

            class PassAttributes {
                std::string      mName;
                std::vector<int> mResourceEventIndices;
                ClearTarget      mClearTarget;
                mce::Color       mClearColor;
            };

            std::unordered_map<const Pass *, PassAttributes> mPassAttributes; // off+0
        };

        // size: 88
        struct ResourceEventRegistry {
            std::vector<ResourceEvent>                          mResourceEventList; // off+0
            std::unordered_map<Resource *, std::pair<int, int>> mResourceLifetimes; // off+24
        };

        RenderOrder          &mOwner;                 // off+0
        PassStorage           mPassStorage;           // off+8
        ResourceStorage       mResourceStorage;       // off+56
        PassRegistry          mPassRegistry;          // off+144
        ResourceEventRegistry mResourceEventRegistry; // off+208
    };

} // namespace rendergraph