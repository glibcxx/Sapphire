#pragma once

#include "SDK/api/src-deps/Core/CheckedResourceService/ServerResourcePointer.h"
#include "SDK/api/src-external/RenderDragon/Resource/DragonBufferResourceService.h"
#include "SDK/api/src-external/RenderDragon/Resource/DragonFrameResources.h"
#include "SDK/api/src-external/RenderDragon/Platform/RenderRegistry.h"
#include "SDK/api/src-external/RenderDragon/Mesh/VertexDeclManager.h"
#include "SDK/api/src-deps/Core/Container/SharedMemoryTracker.h"

namespace MFC {
    struct ActorShadow;
    struct AlphaTestCracks;
    struct Atmosphere;
    struct BlendedCracksObject;
    struct StaticShadowCaster0;
    struct StaticShadowCaster1;
    struct StaticShadowCaster2;
    struct StaticShadowCaster3;
    struct DeferredWater;
    struct EnvironmentalText;
    struct FadeToBlackEffect;
    struct ForceRasterizationItemInHandObject;
    struct Gameface;
    struct GameplayMetadata;
    struct IBLClouds;
    struct InLevelCubeMapObject;
    struct InMenuCubeMapObject;
    struct ItemInHandObject;
    struct ItemShadow;
    struct LivingRoomViewFilter;
    struct LivingRoomTransitionFilter;
    struct LivingRoomSceneObject;
    struct OverlayUI;
    struct ParticleObject;
    struct PersistentUIElement;
    struct PlayerUI;
    struct ColorCorrectedUI;
    struct PlayerVision;
    struct FirstPersonPlayerShadowCaster;
    struct SceneObjectActors;
    struct SceneObjectBanners;
    struct SceneObject;
    struct SceneOverlayObject;
    struct SceneSky;
    struct SceneSkyObject;
    struct SimpleShadow;
    struct SimpleItemShadow;
    struct SelectionOverlayObject;
    struct NonShadowOnlyObject;
    struct ShadowOverlay;
    struct ShadowVolume;
    struct ThirdPersonItemInHandObject;
    struct TransparentItemInWorldObject;
    struct VolumetricFog;
    struct VrConfiguration;
    struct VrPresenceObject;
    struct VrFloatingUiQuad;
    struct WaterHoleObject;
    struct UIActorCapture;
    struct UIStructureVolumeCapture;
    struct UIMeshCapture;
    struct ScreenshotCapture;
    struct NameTag;
    struct EditorHighlig5htConfiguration;
    struct Highlighted;
} // namespace MFC
namespace DFC {
    namespace defaultpasses {
        struct AlphaTest;
        struct Blit;
        struct Opaque;
        struct StencilWrite;
        struct Transparent;
        struct UI;
    } // namespace defaultpasses
    struct ViewSetId;
    struct MeshFilter;
    struct MaterialFilter;
    struct PassState;
    struct PostEffect;
    struct RenderTarget;
    struct Cubemap;
    struct ShadowSource;
    struct ShadowCaster;
    struct ShadowOverlay;
    struct SortIndex;
    struct SortOrigin;
    struct Transform;
    struct UIElement;
    struct ForceRasterization;
    struct ForceRasterizationWithMultiply;
    struct RaytraceObject;
} // namespace DFC
namespace mce::framebuilder::bgfxbridge {
    struct RTXMinecraftFrameRendererResources;
    struct DeferredMinecraftFrameRendererResources;
    struct VanillaMinecraftFrameRendererResources;
    struct HolographicMinecraftFrameRendererResources;
} // namespace mce::framebuilder::bgfxbridge

template <auto Seed = []() {}>
struct UnkRenderComponent {};
static_assert(!std::is_same_v<UnkRenderComponent<>, UnkRenderComponent<>>);

namespace dragon::frameobject {

    // size: 1664
    template <typename... Comps>
    struct Frame {
        Core::SharedMemoryTracker                                               mFrameTrackedMemory;   // off+0
        dragon::DragonFrameResources                                            mFrameResources;       // off+616
        std::unique_ptr<Core::RingAllocatorContainer<uint8_t>::AllocationScope> mFrameAllocationScope; // off+1240
        uint64_t                                                                mUnk1248;              // off+1248

        dragon::mesh::VertexDeclManager            mVertexDeclManager;            // off+1256
        dragon::DragonBufferResourceServiceContext mBufferResourceServiceContext; // off+1264
        dragon::IndexBufferResourceService         mIndexBufferResourceService;   // off+1288
        dragon::VertexBufferResourceService        mVertexBufferResourceService;  // off+1368

        dragon::platform::registry::BasicRenderRegistry<
            DFC::ViewSetId,                      // 89
            DFC::defaultpasses::AlphaTest,       // 88
            DFC::defaultpasses::Blit,            // 87
            DFC::defaultpasses::Opaque,          // 86
            DFC::defaultpasses::StencilWrite,    // 85
            DFC::defaultpasses::Transparent,     // 84
            DFC::defaultpasses::UI,              // 83
            UnkRenderComponent<>,                // 82
            UnkRenderComponent<>,                // 81
            UnkRenderComponent<>,                // 80
            UnkRenderComponent<>,                // 79
            UnkRenderComponent<>,                // 78
            DFC::MeshFilter,                     // 77
            DFC::MaterialFilter,                 // 76
            DFC::PassState,                      // 75
            DFC::PostEffect,                     // 74
            DFC::RenderTarget,                   // 73
            DFC::Cubemap,                        // 72
            DFC::ShadowSource,                   // 71
            DFC::ShadowCaster,                   // 70
            DFC::ShadowOverlay,                  // 69
            DFC::SortIndex,                      // 68
            DFC::SortOrigin,                     // 67
            DFC::Transform,                      // 66
            DFC::UIElement,                      // 65
            DFC::ForceRasterization,             // 64
            DFC::ForceRasterizationWithMultiply, // 63
            UnkRenderComponent<>,                // 62
            DFC::RaytraceObject,                 // 61
            UnkRenderComponent<>,                // 60
            UnkRenderComponent<>,                // 59
            Comps...                             // 0~58
            >
            mRegistry; // off+1448, 90 comps in total (1.21.50)

        std::optional<size_t> mFrameNumber; // off+1624

        std::vector<mce::ServerResourcePointer<dragon::ResolvedShaderBufferResource>> mActiveShaderBuffers; // off+1640

        using TGameRenderRegistry = decltype(mRegistry);

        /*
            during BgfxFrameBuilder init
            a lambda at "continueInit@BgfxFrameBuilder"
            \xE8\x00\x00\x00\x00\x90\x48\x8B\x5C\x24\x00\x48\x83\xC4\x00\x5F\xC3\x33\xC0
        */
        Frame *ctor(dragon::mesh::VertexSpanRegistry &vertexSpanStorage, dragon::mesh::IndexSpanRegistry &indexSpanStorage);
    };

    using GameFrame = Frame<
        MFC::ActorShadow,                                                         // 58
        MFC::AlphaTestCracks,                                                     // 57
        MFC::Atmosphere,                                                          // 56
        MFC::BlendedCracksObject,                                                 // 55
        MFC::StaticShadowCaster0,                                                 // 54
        MFC::StaticShadowCaster1,                                                 // 53
        MFC::StaticShadowCaster2,                                                 // 52
        MFC::StaticShadowCaster3,                                                 // 51
        MFC::DeferredWater,                                                       // 50
        MFC::EnvironmentalText,                                                   // 49
        MFC::FadeToBlackEffect,                                                   // 48
        MFC::ForceRasterizationItemInHandObject,                                  // 47
        MFC::Gameface,                                                            // 46
        MFC::GameplayMetadata,                                                    // 45
        MFC::IBLClouds,                                                           // 44
        MFC::InLevelCubeMapObject,                                                // 43
        MFC::InMenuCubeMapObject,                                                 // 42
        MFC::ItemInHandObject,                                                    // 41
        MFC::ItemShadow,                                                          // 40
        MFC::LivingRoomViewFilter,                                                // 39
        MFC::LivingRoomTransitionFilter,                                          // 38
        MFC::LivingRoomSceneObject,                                               // 37
        MFC::OverlayUI,                                                           // 36
        MFC::ParticleObject,                                                      // 35
        MFC::PersistentUIElement,                                                 // 34
        MFC::PlayerUI,                                                            // 33
        MFC::ColorCorrectedUI,                                                    // 32
        MFC::PlayerVision,                                                        // 31
        MFC::FirstPersonPlayerShadowCaster,                                       // 30
        MFC::SceneObjectActors,                                                   // 29
        MFC::SceneObjectBanners,                                                  // 28
        MFC::SceneObject,                                                         // 27
        MFC::SceneOverlayObject,                                                  // 26
        MFC::SceneSky,                                                            // 25
        MFC::SceneSkyObject,                                                      // 24
        MFC::SimpleShadow,                                                        // 23
        MFC::SimpleItemShadow,                                                    // 22
        MFC::SelectionOverlayObject,                                              // 21
        MFC::NonShadowOnlyObject,                                                 // 20
        MFC::ShadowOverlay,                                                       // 19
        MFC::ShadowVolume,                                                        // 18
        MFC::ThirdPersonItemInHandObject,                                         // 17
        MFC::TransparentItemInWorldObject,                                        // 16
        MFC::VolumetricFog,                                                       // 15
        MFC::VrConfiguration,                                                     // 14
        MFC::VrPresenceObject,                                                    // 13
        MFC::VrFloatingUiQuad,                                                    // 12
        MFC::WaterHoleObject,                                                     // 11
        MFC::UIActorCapture,                                                      // 10
        MFC::UIStructureVolumeCapture,                                            // 9
        MFC::UIMeshCapture,                                                       // 8
        MFC::ScreenshotCapture,                                                   // 7
        MFC::NameTag,                                                             // 6
        MFC::EditorHighlig5htConfiguration,                                       // 5
        MFC::Highlighted,                                                         // 4
        mce::framebuilder::bgfxbridge::RTXMinecraftFrameRendererResources,        // 3
        mce::framebuilder::bgfxbridge::DeferredMinecraftFrameRendererResources,   // 2
        mce::framebuilder::bgfxbridge::VanillaMinecraftFrameRendererResources,    // 1
        mce::framebuilder::bgfxbridge::HolographicMinecraftFrameRendererResources // 0
        >;
    static_assert(std::tuple_size_v<GameFrame::TGameRenderRegistry::ComponentType> == 90);

} // namespace dragon::frameobject