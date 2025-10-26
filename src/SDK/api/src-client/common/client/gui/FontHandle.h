#pragma once

#include "SDK/api/src-deps/Core/Utility/EnableNonOwnerReferences.h"

class FontRepository;
class Font;

// size: 80 (1.21.50)
class FontHandle : public Bedrock::EnableNonOwnerReferences {
    uint8_t _fill24[80 - 24];

    FontHandle(const FontHandle &other); // \xE8\x00\x00\x00\x00\x48\x8B\xF8\x48\x8B\xD0\x48\x8D\x4E (1.21.50)
};