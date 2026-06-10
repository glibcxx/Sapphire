#pragma once

#include "pch.h" // IWYU pragma: keep

#include "TextureAtlasItem.h"

class AtlasItemManager {
public:
    std::unordered_map<std::string, TextureAtlasItem>        mTextureAtlasItems;
    std::vector<class TextureAtlasItemTextureSetTranslation> mTextureAtlasItemTextureSetTranslations;

    AtlasItemManager() = delete;
};
