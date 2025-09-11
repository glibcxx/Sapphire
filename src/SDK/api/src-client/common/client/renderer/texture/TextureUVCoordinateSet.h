#pragma once

#include "IsotropicFaceData.h"
#include "SDK/api/src-deps/Core/Resource/ResourceHelper.h"

// size: 96
struct TextureUVCoordinateSet {
    float             weight;
    float             _u0;
    float             _v0;
    float             _u1;
    float             _v1;
    uint16_t          _texSizeW;
    uint16_t          _texSizeH;
    ResourceLocation  sourceFileLocation;
    IsotropicFaceData mIsotropicFaceData;
    int16_t           textureSetTranslationIndex;
    uint16_t          mPBRTextureDataHandle;

    float width() const { return this->_u1 - this->_u0; }
    float height() const { return this->_v1 - this->_v0; }
};
