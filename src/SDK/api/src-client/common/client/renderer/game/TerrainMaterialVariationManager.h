#pragma once

class TerrainLayer;

class TerrainMaterialVariationManager {};

inline namespace anonymous_namespace {

    SPHR_DECL_API("1.21.2,1.21.50,1.21.60", "call", "\xE8\x00\x00\x00\x00\x90\x48\x8B\x4D\x00\x48\x8B\x19\x44\x38")
    SDK_API std::map<TerrainLayer *, std::string_view> getMaterialMapPairs();

} // namespace anonymous_namespace
