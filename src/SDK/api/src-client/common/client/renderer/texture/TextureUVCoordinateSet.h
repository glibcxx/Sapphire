#pragma once

#include "IsotropicFaceData.h"

#include "SDK/api/src-deps/Core/Resource/ResourceHelper.h"

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
    uint16_t          mUnk1;
    uint16_t          mUnk2;

    float width() const { return this->_u1 - this->_u0; }
    float height() const { return this->_v1 - this->_v0; }
};
