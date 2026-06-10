#pragma once

#include "pch.h" // IWYU pragma: keep

#include "TextureUVCoordinateSet.h"

class TextureAtlasItem {
public:
    std::string mName;
    int         mParsedNodeIndex;

    std::vector<std::vector<TextureUVCoordinateSet>> mTextureUVs;
};
