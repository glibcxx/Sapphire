#pragma once

#include "SDK/api/src-deps/Core/CheckedResourceService/ServerResourcePointer.h"
#include "SDK/api/src-deps/Coregraphics/Math/Rect.h"
#include "SDK/api/src-external/RenderDragon/Materials/Definition/MaterialShared.h"
#include "SDK/api/src-external/bgfx/bgfx.h"

namespace dragon {
    namespace mesh {
        class TypedVertexBufferHandle;
    }
    namespace platform {
        class ResourcesManager;
    }
    namespace rendering {
        class CommandContext;
        struct GPUState;
    } // namespace rendering
    namespace materials {
        class Pass;
        class MaterialUniformOverrides;
        struct PassProgramHandle;
    } // namespace materials
} // namespace dragon
namespace DFC {
    struct MeshFilter;
    struct MaterialFilter;
    struct Transform;
} // namespace DFC

namespace dragon::rendering::drawcommands {

    struct GraphicsCommandContext;
    struct TopLevelInstanceDescription;

    // size: 20
    struct TextureBinding {
        // size: 12
        struct Image {
            uint8_t                   mMip;    // off+0
            bgfx::Access::Enum        mAccess; // off+4
            bgfx::TextureFormat::Enum mFormat; // off+8
        };

        // size: 8
        struct Texture {
            bgfx::UniformHandle mSampler; // off+0
            uint32_t            mFlags;   // off+4
        };

        bgfx::TextureHandle                          mHandle; // off+0
        std::variant<std::monostate, Image, Texture> mData;   // off+4
    };

    // size: 24
    struct UniformBindings {
        bgfx::UniformHandle mHandle; // off+0
        const void         *mValue;  // off+8
        uint16_t            mNum;    // off+16
    };

    // size: 16
    struct BufferBinding {
        // size: 4
        struct ShaderBufferHandle {
            bgfx::UniformHandle      mSampler; // off+0
            bgfx::ShaderBufferHandle mHandle;  // off+2
        };

        uint8_t mStage; // off+0
        std::variant<
            bgfx::IndexBufferHandle,
            bgfx::VertexBufferHandle,
            bgfx::DynamicIndexBufferHandle,
            bgfx::DynamicVertexBufferHandle,
            bgfx::IndirectBufferHandle,
            ShaderBufferHandle>
                           mHandle; // off+2
        bgfx::Access::Enum mAccess; // off+8
        uint32_t           mUnk12;  // off+12
    };

    // size: 128 (1.21.50)
    struct GraphicsCommandContext {
        dragon::rendering::CommandContext         &mCommands; // off+0
        std::vector<std::optional<TextureBinding>> mTextures; // off+8
        std::vector<UniformBindings>               mUniforms; // off+32
        std::vector<std::vector<BufferBinding>>    mBuffers;  // off+56
        std::vector<std::aligned_storage_t<6, 2>>  mUnk80;    // off+80
        std::vector<std::aligned_storage_t<8, 4>>  mUnk104;   // off+104
    };

    // size: 128 (1.21.50)
    struct DrawCommandContext : GraphicsCommandContext {};

    DrawCommandContext createDrawContext(dragon::rendering::CommandContext &commands);

    bool createTopLevelAccelerationStructure(
        dragon::rendering::CommandContext &commands, const std::vector<TopLevelInstanceDescription> &tlasDescription
    );

    void debugMarker(GraphicsCommandContext &commands, const char *name);

    SDK_API void applyState(
        DrawCommandContext                &commands,
        const cg::math::Rect<uint16_t>    &scissorRect,
        const dragon::rendering::GPUState &state
    );

    SDK_API bool bindMesh(DrawCommandContext &commands, const DFC::MeshFilter &meshFilter);

    bool bindMeshIndexBuffer(DrawCommandContext &commands, const DFC::MeshFilter &meshFilter);

    SDK_API bool extractUniforms(
        GraphicsCommandContext                            &commands,
        dragon::materials::definition::ShaderPipeline      pipeline,
        const DFC::MaterialFilter                         &materialFilter,
        const dragon::materials::MaterialUniformOverrides *uniformOverrides
    );

    SDK_API void bindUniforms(GraphicsCommandContext &commands);

    // partial inlined
    SDK_API bool submit(
        DrawCommandContext            &commands,
        bool                           preserveState,
        bgfx::ViewId                   id,
        const DFC::Transform          &transform,
        const dragon::materials::Pass &pass,
        int                            depth = 0
    );

    SDK_API bool submit(
        DrawCommandContext            &commands,
        bool                           preserveState,
        bgfx::ViewId                   id,
        const glm::tmat4x4<float>     &transform,
        const dragon::materials::Pass &pass,
        int                            depth = 0
    );

    SDK_API bool submit(
        DrawCommandContext                         &commands,
        bool                                        preserveState,
        bgfx::ViewId                                id,
        const glm::tmat4x4<float>                  &transform,
        const dragon::materials::PassProgramHandle &program,
        int                                         depth = 0
    );

    bool _submitArrayAsIndividualElements(
        DrawCommandContext                         &commands,
        bool                                        preserveState,
        bgfx::ViewId                                id,
        gsl::span<const glm::tmat4x4<float>>        transforms,
        const dragon::materials::PassProgramHandle &program,
        int                                         depth = 0
    );

    SDK_API bool submitInstanced(
        DrawCommandContext                         &commands,
        bool                                        preserveState,
        bgfx::ViewId                                id,
        size_t                                      instanceCount,
        const dragon::materials::PassProgramHandle &program,
        int                                         depth = 0
    );

    SDK_API bool submitInstanced(
        DrawCommandContext                  &commands,
        bool                                 preserveState,
        bgfx::ViewId                         id,
        gsl::span<const glm::tmat4x4<float>> transforms,
        const dragon::materials::Pass       &pass,
        int                                  depth = 0
    );

    SDK_API bool submitInstanced(
        DrawCommandContext                         &commands,
        bool                                        preserveState,
        bgfx::ViewId                                id,
        gsl::span<const glm::tmat4x4<float>>        transforms,
        const dragon::materials::PassProgramHandle &program,
        const dragon::materials::PassProgramHandle &fallbackProgram,
        int                                         depth = 0
    );

} // namespace dragon::rendering::drawcommands