#pragma once

#include <unordered_map>
#include "TextureAtlasItem.h"

class AtlasItemManager {
public:
    std::unordered_map<std::string, TextureAtlasItem>        mTextureAtlasItems;
    std::vector<class TextureAtlasItemTextureSetTranslation> mTextureAtlasItemTextureSetTranslations;

    AtlasItemManager() = delete;
};
