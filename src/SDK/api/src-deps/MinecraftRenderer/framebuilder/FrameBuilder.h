#pragma once

#include <winrt/impl/Windows.UI.Core.2.h>
#include "FrameLightingModelCapabilities.h"
#include "SDK/api/src-deps/Core/Utility/EnableNonOwnerReferences.h"
#include "SDK/api/src-external/RenderDragon/Mesh/IndexSize.h"
#include "SDK/api/src-external/RenderDragon/Platform/Statistics.h"
#include "SDK/api/src-deps/Renderer/hal/enums/RenderFeature.h"
#include "SDK/api/src-external/RenderDragon/Platform/VerticalSync.h"
#include "SDK/api/src-external/RenderDragon/Rendering/ClipSpaceOrigin.h"
#include "SDK/api/src-external/RenderDragon/Rendering/DeferredResult.h"
#include "SDK/api/src-external/RenderDragon/Platform/TextureWrapper.h"
#include "SDK/api/src-external/RenderDragon/Resource/DragonBufferResourceService.h"
#include "SDK/api/src-external/RenderDragon/Resource/DragonMaterialResourceService.h"
#include "SDK/api/src-external/RenderDragon/Resource/DragonTextureResourceService.h"

struct RenderChunkDirectVertexData;
struct RenderChunkDirectIndexData;
struct RangeIndices;
namespace mce {
    class ClientTexture;
    class VertexFormat;
} // namespace mce
namespace dragon::rendering {
    class SharedTextureHandle;
}
namespace dragon {
    class MaterialResourceManager;
    struct RenderMetadata;
} // namespace dragon
enum class LevelCullerType : uint8_t;
enum class FrustumCullerType : uint8_t;

// size: 24
struct WinRtOpenXrSurfaceParameters {
    __int64 mUnk0;
    __int64 mUnk8;
    __int8  mUnk16;
    __int8  mUnk17;
};

namespace mce::framebuilder {

    namespace bgfxbridge {
        struct EntityCreationContext;
    }
    class FrameBuilderContext;
    struct CameraDescriptionOrthographic;
    struct CameraDescriptionPerspective;
    struct BufferClearDescription;

    enum class AsyncLoadResult : int32_t {
        Pending = 0,
        Completed = 1,
    };

    // size: 24
    class FrameBuilder : public Bedrock::EnableNonOwnerReferences {
    public:
        // size: 8
        class InitializationState {
        public:
            // vtb+0
            virtual ~InitializationState();
        };

        // vtb+0
        virtual ~FrameBuilder() override;

        // vtb+1
        virtual bool enabled() const = 0;

        // vtb+2
        virtual std::optional<dragon::platform::Statistics> getStatistics() = 0;

        // vtb+3
        virtual bool supportsRenderFeature(const mce::RenderFeature) const = 0;

        // vtb+4
        virtual double *unk_virtual_4(double *result) = 0;

        // vtb+5
        virtual bool isDeviceRemoved();

        // vtb+6
        virtual std::optional<std::shared_ptr<InitializationState>> init(
            const std::variant<
                winrt::agile_ref<winrt::Windows::UI::Core::CoreWindow>,
                WinRtOpenXrSurfaceParameters,
                std::monostate> &surfaceParams,
            const uint32_t       screenWidth,
            const uint32_t       screenHeight,
            void *,
            std::function<void()>
        ) = 0;

        // vtb+7
        virtual std::optional<std::shared_ptr<InitializationState>> continueInit(
            std::shared_ptr<InitializationState> initState
        ) = 0;

        // vtb+8
        virtual bool initialized() const = 0;

        // vtb+9
        virtual void destroy() = 0;

        // vtb+10
        virtual void loadCoreAssets(
            const std::string &basePath, std::function<std::string(const std::string &)> loadFileDataCallback
        ) = 0;

        // vtb+11
        virtual AsyncLoadResult loadCompiledMaterial();

        // vtb+12
        virtual void startFrame() = 0;

        // vtb+13
        virtual void endFrame(FrameBuilderContext &&frameBuilderContext) = 0;

        // vtb+14
        virtual void discardFrame(bool) = 0;

        // vtb+15
        virtual void setTerrainAtlasTexture(const mce::ClientTexture &texture) = 0;

        // vtb+16
        virtual void setDefaultActorMERSUniforms(float metalness, float emissive, float roughness, float sss) = 0;

        // vtb+17
        virtual void setDefaultParticleMERSUniforms(float metalness, float emissive, float roughness, float sss);

        // vtb+18
        virtual void updateSurfaceParameters(
            const std::variant<
                winrt::agile_ref<winrt::Windows::UI::Core::CoreWindow>,
                WinRtOpenXrSurfaceParameters,
                std::monostate> &surfaceParams
        ) = 0;

        // vtb+19
        virtual void updateWindowSize(const uint32_t width, const uint32_t height) = 0;

        // vtb+20
        virtual void updateClientViewSize(const glm::tvec2<float> &clientViewSize) = 0;

        // vtb+21
        virtual bool updateMsaaLevel(const uint8_t level) = 0;

        // vtb+22
        virtual void updateVSync(dragon::platform::VerticalSync verticalSync) = 0;

        // vtb+23
        virtual void unk_virtual_23() = 0;

        // vtb+24
        virtual void unk_virtual_24() = 0;

        // vtb+25
        virtual void unk_virtual_25() = 0;

        // vtb+26
        virtual void unk_virtual_26() = 0;

        // vtb+27
        virtual dragon::rendering::DeferredResult<dragon::rendering::SharedTextureHandle> createExternalTexture(
            std::optional<std::variant<
                std::monostate,
                dragon::platform::GLTextureWrapper,
                dragon::platform::WindowsTextureWrapper>> nativeTexture
        ) = 0;

        // vtb+28
        virtual void updateExternalTexture(
            dragon::rendering::DeferredResult<dragon::rendering::SharedTextureHandle> texture,
            std::optional<std::variant<
                std::monostate,
                dragon::platform::GLTextureWrapper,
                dragon::platform::WindowsTextureWrapper>>                             nativeTexture,
            std::function<void(dragon::rendering::SharedTextureHandle)>               rendererCallbackUpdateDelegate
        ) = 0;

        // vtb+29
        virtual dragon::ServerTexture wrapExternalTexture(
            dragon::platform::RenderAPI api, const dragon::TextureDescription &desc, void *externalTexture
        ) = 0;

        // vtb+30
        virtual void createRenderChunk(const dragon::RenderMetadata &renderMetadata) = 0;

        // vtb+31
        virtual void destroyRenderChunk(const dragon::RenderMetadata &renderMetadata) = 0;

        // vtb+32
        virtual void generateRenderChunkVertexData(
            RenderChunkDirectVertexData        &renderChunkDirectVertexData,
            const std::array<RangeIndices, 18> &layerRanges,
            const gsl::span<const uint8_t>     &chunkData,
            const uint64_t                     &vertexCount,
            const mce::VertexFormat            &mceFormat,
            const int                          *absoluteBlockPosition
        ) = 0;

        // vtb+33
        virtual void generateRenderChunkIndexData(
            RenderChunkDirectIndexData         &renderChunkDirectIndexData,
            const std::array<RangeIndices, 18> &layerRanges,
            const gsl::span<const uint8_t>     &indexData,
            dragon::mesh::IndexSize             indexSize
        ) = 0;

        // vtb+34
        virtual void freeRenderChunkVertexData(RenderChunkDirectVertexData &renderChunkDirectVertexData) = 0;

        // vtb+35
        virtual void freeRenderChunkIndexData(RenderChunkDirectIndexData &renderChunkDirectIndexData) = 0;

        // vtb+36
        virtual void debugPrintToScreen(std::function<void(const char *)> printFunction) = 0;

        // vtb+37
        virtual void debugDeclareSceneCamera(
            const mce::framebuilder::CameraDescriptionOrthographic &cameraDescription,
            const mce::framebuilder::BufferClearDescription        &bufferClearDescription
        ) = 0;

        // vtb+38
        virtual void debugDeclareSceneCamera(
            const mce::framebuilder::CameraDescriptionPerspective &cameraDescription,
            const mce::framebuilder::BufferClearDescription       &bufferClearDescription
        ) = 0;

        // vtb+39
        virtual void *unk_virtual_39(void *result) = 0;

        // vtb+40
        virtual void *unk_virtual_40(void *result) = 0;

        // vtb+41
        virtual void *unk_virtual_41(void *result) = 0;

        // vtb+42
        virtual void *unk_virtual_42(void *result) = 0;

        // vtb+43
        virtual dragon::rendering::ClipSpaceOrigin getClipSpaceOrigin() const = 0;

        // vtb+44
        virtual dragon::VertexBufferResourceService *getVertexBufferResourceService() = 0;

        // vtb+45
        virtual dragon::IndexBufferResourceService *getIndexBufferResourceService() = 0;

        // vtb+46
        virtual dragon::ShaderBufferResourceService *getShaderBufferResourceService() = 0;

        // vtb+47
        virtual dragon::TextureResourceService *getTextureResourceService() = 0;

        // vtb+48
        virtual dragon::MaterialResourceService *getMaterialResourceService() = 0;

        // vtb+49
        virtual dragon::Unk1768ResourceService *unk_virtual_49() = 0;

        // vtb+50
        virtual dragon::MaterialResourceManager *getMaterialResourceManager() = 0;

        // vtb+51
        virtual FrameLightingModelCapabilities getLightingModelCapabilities() = 0;

        // vtb+52
        virtual bool isRayTracingCapable() const = 0;

        // vtb+53
        virtual bool isRayTracingEnabled() const = 0;

        // vtb+54
        virtual bool isDeferredRenderingCapable() const = 0;

        // vtb+55
        virtual bool isDeferredRenderingEnabled() const = 0;

        // vtb+56
        virtual bool unk_virtual_56() const = 0;

        // vtb+57
        virtual bool unk_virtual_57() const = 0;

        // vtb+58
        virtual std::shared_ptr<void> unk_virtual_58() = 0;

        // vtb+59
        virtual std::shared_ptr<void> unk_virtual_59() const = 0;

        // vtb+60
        virtual std::shared_ptr<void> unk_virtual_60() = 0;

        // vtb+61
        virtual std::shared_ptr<void> unk_virtual_61() const = 0;

        // vtb+62
        virtual std::shared_ptr<void> unk_virtual_62() = 0;

        // vtb+63
        virtual void updateLightingModels(dragon::rendering::LightingModels model, bool &changed) = 0;

        // vtb+64
        virtual glm::tvec2<uint16_t> getResolution() = 0;

        // vtb+65
        virtual bool isUpscalingAvailable() const = 0;

        // vtb+66
        virtual void setUpscaling(bool upscale) = 0;

        // vtb+67
        virtual bool isUpscalingEnabled() const = 0;

        // vtb+68
        virtual float getUpscalingFactor() const = 0;

        // vtb+69
        virtual const std::string_view getUpscalingInfo() const = 0;

        // vtb+70
        virtual void custom(const std::function<void(const bgfxbridge::EntityCreationContext &)> &insertCallback) = 0;

        // vtb+71
        virtual void queueGraphicsTask(std::function<void()> &&task);

        // vtb+72
        virtual LevelCullerType getLevelCullerType() const = 0;

        // vtb+73
        virtual FrustumCullerType getFrustumCullerType() const = 0;

        // vtb+74
        virtual char unk_virtual_74() = 0;

        // vtb+75
        virtual void clearRenderingResourcesCache() = 0;

        // vtb+76
        virtual void unk_virtual_76(void *) = 0;

        // vtb+77
        virtual bool unk_virtual_77(uint64_t) const = 0;

        // vtb+78
        virtual void unk_virtual_78() = 0;

        // vtb+79
        virtual void unk_virtual_79() = 0;

        // vtb+80
        virtual void registerWindowHandle(
            class std::variant<
                struct winrt::agile_ref<struct winrt::Windows::UI::Core::CoreWindow>,
                struct WinRtOpenXrSurfaceParameters,
                struct std::monostate> const &,
            uint16_t handle,
            int,
            int
        ) = 0;

        // vtb+81
        virtual void unregisterWindowHandle(uint16_t handle);

        // vtb+82
        virtual void unk_virtual_82(void *) = 0;

        // vtb+83
        virtual void _insert(void *description) = 0;

        // vtb+84
        virtual void _insert_1(void *description) = 0;

        // vtb+85
        virtual void _insert_2(void *description) = 0;

        // vtb+86
        virtual void _insert_3(void *description) = 0;

        // vtb+87
        virtual void _insert_4(void *description) = 0;

        // vtb+88
        virtual void _insert_5(void *description) = 0;

        // vtb+89
        virtual void _insert_6(void *description) = 0;

        // vtb+90
        virtual void unk_virtual_90() = 0;
    };

} // namespace mce::framebuilder