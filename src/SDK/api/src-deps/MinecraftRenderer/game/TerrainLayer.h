#pragma once

#include "pch.h" // IWYU pragma: keep

// size: 56
class TerrainLayer {
public:
    enum class Transparency : int {
        None = 0,
        Sorted = 1,
    };

    enum class Detail : int {
        Near = 0,
        Far = 1,
    };

    const size_t       id;            // off+0
    const Transparency transparency;  // off+8
    const std::string  name;          // off+16
    size_t             triangleCount; // off+48

    // SDK_API static TerrainLayer Opaque;
    // SDK_API static TerrainLayer EndPortal;
    // SDK_API static TerrainLayer Barrier;
    // SDK_API static TerrainLayer Light;
    // SDK_API static TerrainLayer StructureVoid;
    // SDK_API static TerrainLayer OpaqueSeasons;
    // SDK_API static TerrainLayer DoubleSide;
    // SDK_API static TerrainLayer Alpha;
    // SDK_API static TerrainLayer AlphaSingleSide;
    // SDK_API static TerrainLayer AlphaSeasons;
    // SDK_API static TerrainLayer Blend;
    // SDK_API static TerrainLayer DeferedWater;
    // SDK_API static TerrainLayer RayTracedWater;
    // SDK_API static TerrainLayer BlendFar;
    // SDK_API static TerrainLayer Far;
    // SDK_API static TerrainLayer FarSeasons;
    // SDK_API static TerrainLayer FarSeasonsAlpha;
    // SDK_API static TerrainLayer LightSource;

    class TerrainLayers;

    SPHR_DECL_API("1.21.50", "disp:4,lea", "\x48\x8D\x4B\x00\x4C\x8D\x05")
    SDK_API static TerrainLayers sTerrainLayers;

    SPHR_DECL_API("1.21.2", "mov", "\x48\x8B\x15\x00\x00\x00\x00\x48\x3B\x15\x00\x00\x00\x00\x74\x00\x48\x89\x1A\x48\x83\x05\x00\x00\x00\x00\x00\xEB\x00\x48\x8D\x0D\x00\x00\x00\x00\xEB")
    SPHR_DECL_API("1.21.50,1.21.60", "mov", "\x48\x8B\x0D\x00\x00\x00\x00\x49\x8B\xD6")
    SDK_API static std::vector<TerrainLayer *> sAllLayers;
    SPHR_DECL_API("1.21.2,1.21.50,1.21.60", "mov,disp:-8", "\x48\x8B\x05\x00\x00\x00\x00\x48\x2B\xC3\x48\xC1\xF8\x00\x48\x8D\x3C\xC3")
    SDK_API static std::vector<TerrainLayer *> sNearLayers;
};

class TerrainLayer::TerrainLayers {
public:
    TerrainLayer Opaque;
    TerrainLayer EndPortal;
    TerrainLayer Barrier;
    TerrainLayer Light;
    TerrainLayer StructureVoid;
    TerrainLayer OpaqueSeasons;
    TerrainLayer DoubleSide;
    TerrainLayer Alpha;
    TerrainLayer AlphaSingleSide;
    TerrainLayer AlphaSeasons;
    TerrainLayer Blend;
    TerrainLayer DeferedWater;
    TerrainLayer RayTracedWater;
    TerrainLayer BlendFar;
    TerrainLayer Far;
    TerrainLayer FarSeasons;
    TerrainLayer FarSeasonsAlpha;
    TerrainLayer LightSource;
};
