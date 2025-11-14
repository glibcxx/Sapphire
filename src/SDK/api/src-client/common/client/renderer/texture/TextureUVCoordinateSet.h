#pragma once

#include "IsotropicFaceData.h"
#include "SDK/api/src-deps/Core/Resource/ResourceHelper.h"

// size: 96
struct TextureUVCoordinateSet {
    float             weight;                     // off+0
    float             _u0;                        // off+4
    float             _v0;                        // off+8
    float             _u1;                        // off+12
    float             _v1;                        // off+16
    uint16_t          _texSizeW;                  // off+20
    uint16_t          _texSizeH;                  // off+22
    ResourceLocation  sourceFileLocation;         // off+24
    IsotropicFaceData mIsotropicFaceData;         // off+80
    int16_t           textureSetTranslationIndex; // off+88
    uint16_t          mPBRTextureDataHandle;      // off+90

    float width() const { return this->_u1 - this->_u0; }
    float height() const { return this->_v1 - this->_v0; }
};
