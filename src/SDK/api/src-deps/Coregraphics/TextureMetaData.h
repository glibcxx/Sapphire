#pragma once

namespace cg {

    // size: 32
    struct TextureMetaData {
        enum class FilterMode : int {
            Filter_Point = 0,
            Filter_Linear = 1,
            Filter_Anisotropic = 2,
        };

        enum class ColorSpace : int {
            ColorSpace_Gamma = 0,
            ColorSpace_Linear = 1,
        };

        enum class Dimension : int {
            Dimension_2D = 0,
            Dimension_3D = 1,
            Dimension_Cube = 2,
        };

        enum class WrapMode : int {
            WrapMode_Clamp = 0,
            WrapMode_Wrap = 1,
            WrapMode_Mirror = 2,
        };

        FilterMode mFilterMin;  // off+0
        FilterMode mFilterMag;  // off+4
        FilterMode mFilterMip;  // off+8
        ColorSpace mColorSpace; // off+12
        Dimension  mDimension;  // off+16
        WrapMode   mWrapU;      // off+20
        WrapMode   mWrapV;      // off+24
        int        mNumSlices;  // off+28
    };

} // namespace cg