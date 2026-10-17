#pragma once

#include "SDK/api/src/common/world/level/dimension/DimensionType.h"

class LevelData;

// size: 1344 (1.21.2), 1376 (1.21.50), 1352 (1.21.60)
class LevelSettings {
public:
    SDK_API LevelSettings();

    SDK_API LevelSettings(const LevelData &data, DimensionType dimension);

    SPHR_DECL_API("1.21.2,1.21.50,1.21.60", "disp:7,call", "\x48\x89\x01\x48\x83\xC1\x00\xE8\x00\x00\x00\x00\x90\x48\xC7\x87")
    SPHR_CTOR_ALIAS SDK_API LevelSettings *ctor();

    SPHR_DECL_API("1.21.2", "disp:58,call", "\x49\x8B\x36\x4C\x8B\xC7")
    SPHR_DECL_API("1.21.50,1.21.60", "disp:58,call", "\x49\x8B\x3F\x4D\x8B\xC6")
    SPHR_CTOR_ALIAS SDK_API LevelSettings *ctor(const LevelData &data, DimensionType dimension);
};