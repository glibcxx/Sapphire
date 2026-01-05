#pragma once

#include "SDK/api/src-deps/MinecraftRenderer/renderer/RenderingProfile.h"

class TerrainLayer;
namespace mce {
    class MaterialPtr;
    class RenderMaterialGroup;
} // namespace mce

enum class TerrainVariation : int {
    Base = 0,
    Fog = 1,
    Lit = 2,
    Fading = 3,
    Shadow = 4,
    Underwater = 5,
    Underlava = 6,
    Water = 7,
    Count = 8,
};

// size: 1
class MaterialVariationManager {};

// size: 144
class TerrainMaterialVariationManager : public MaterialVariationManager {
public:
    using TerrainLayerMaterialMap = std::map<const TerrainLayer *, mce::MaterialPtr>;

    mce::RenderingProfile                                       mRenderingProfile;        // off+0
    mce::RenderMaterialGroup                                   *mRenderMaterialGroup;     // off+8
    std::string                                                 mVariationName;           // off+16
    std::string                                                 mFullVariationName;       // off+48
    std::string                                                 mFullParentVariationName; // off+80
    std::map<TerrainVariation, TerrainMaterialVariationManager> mSubVariations;           // off+112
    TerrainLayerMaterialMap                                     terrainLayerMaterialMap;  // off+128
};

inline namespace anonymous_namespace {

    SPHR_DECL_API("1.21.2,1.21.50,1.21.60", "call", "\xE8\x00\x00\x00\x00\x90\x48\x8B\x4D\x00\x48\x8B\x19\x44\x38")
    SDK_API std::map<TerrainLayer *, std::string_view> getMaterialMapPairs();

} // namespace anonymous_namespace
