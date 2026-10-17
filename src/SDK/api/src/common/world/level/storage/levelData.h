#pragma once

class LevelData {
public:
    SDK_API LevelData(bool isEduMode);

    SPHR_DECL_API("1.21.2,1.21.50,1.21.60", "disp:16,call", "\xFF\x15\x00\x00\x00\x00\x0F\xB6\xD0\x48\x8D\x8D\x00\x00\x00\x00\xE8\x00\x00\x00\x00\x90\x48\x8D\x8D")
    SPHR_CTOR_ALIAS SDK_API LevelData *ctor(bool isEduMode);
};