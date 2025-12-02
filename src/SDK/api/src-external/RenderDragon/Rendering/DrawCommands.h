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

    SPHR_DECL_API("1.21.50", "\x48\x89\x5C\x24\x00\x57\x48\x83\xEC\x00\x41\x80\x78\x00\x00\x48\x8B\xF9") // 1.21.50
    SDK_API void applyState(
        DrawCommandContext                &commands,
        const cg::math::Rect<uint16_t>    &scissorRect,
        const dragon::rendering::GPUState &state
    );

    SPHR_DECL_API("1.21.50", "call", "\xE8\x00\x00\x00\x00\x84\xC0\x0F\x84\x00\x00\x00\x00\x48\x8D\x56\x00\x48\x8D\x8D") // 1.21.50
    SDK_API bool bindMesh(DrawCommandContext &commands, const DFC::MeshFilter &meshFilter);

    bool bindMeshIndexBuffer(DrawCommandContext &commands, const DFC::MeshFilter &meshFilter);

    SPHR_DECL_API("1.21.50", "call", "\xE8\x00\x00\x00\x00\x84\xC0\x74\x00\x49\x8B\x06\x48\x8B\x08") // 1.21.50
    SDK_API bool extractUniforms(
        GraphicsCommandContext                            &commands,
        dragon::materials::definition::ShaderPipeline      pipeline,
        const DFC::MaterialFilter                         &materialFilter,
        const dragon::materials::MaterialUniformOverrides *uniformOverrides
    );

    SPHR_DECL_API("1.21.50", "\x48\x8B\xC4\x53\x56\x57\x48\x81\xEC\x00\x00\x00\x00\x48\x8B\x39") // 1.21.50
    SDK_API void bindUniforms(GraphicsCommandContext &commands);

    // partial inlined
    SPHR_DECL_API("1.21.50", "\x48\x83\xEC\x00\x49\x0F\xBE\x41\x00\x4C\x8B\xD9") // 1.21.50
    SDK_API bool submit(
        DrawCommandContext            &commands,
        bool                           preserveState,
        bgfx::ViewId                   id,
        const DFC::Transform          &transform,
        const dragon::materials::Pass &pass,
        int                            depth = 0
    );

    SPHR_DECL_API("1.21.50", "\x48\x89\x5C\x24\x00\x55\x56\x57\x41\x56\x41\x57\x48\x83\xEC\x00\x4D\x8B\xF9\x45\x0F\xB7\xF0") // 1.21.50
    SDK_API bool submit(
        DrawCommandContext            &commands,
        bool                           preserveState,
        bgfx::ViewId                   id,
        const glm::tmat4x4<float>     &transform,
        const dragon::materials::Pass &pass,
        int                            depth = 0
    );

    SPHR_DECL_API("1.21.50", "\x40\x55\x56\x41\x56\x41\x57\x48\x83\xEC\x00\x48\x8B\xB4\x24") // 1.21.50
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

    SPHR_DECL_API("1.21.50", "\x4C\x8B\xDC\x55\x57\x41\x54\x41\x56\x41\x57") // 1.21.50
    SDK_API bool submitInstanced(
        DrawCommandContext                         &commands,
        bool                                        preserveState,
        bgfx::ViewId                                id,
        size_t                                      instanceCount,
        const dragon::materials::PassProgramHandle &program,
        int                                         depth = 0
    );

    SPHR_DECL_API("1.21.50", "call", "\xE8\x00\x00\x00\x00\xE9\x00\x00\x00\x00\x44\x89\x74\x24\x00\x48\x89\x54\x24") // 1.21.50
    SDK_API bool submitInstanced(
        DrawCommandContext                  &commands,
        bool                                 preserveState,
        bgfx::ViewId                         id,
        gsl::span<const glm::tmat4x4<float>> transforms,
        const dragon::materials::Pass       &pass,
        int                                  depth = 0
    );

    SPHR_DECL_API("1.21.50", "call", "\xE8\x00\x00\x00\x00\xE9\x00\x00\x00\x00\x80\xBB\x00\x00\x00\x00\x00\x74") // 1.21.50
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