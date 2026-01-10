#pragma once

#include "SDK/api/src-deps/Core/Math/Color.h"
#include "SDK/api/src-deps/Core/Utility/StringHash.h"
#include "SDK/api/src-deps/Renderer/Matrix.h"
#include "SDK/api/src/common/world/phys/Vec4.h"
#include "SDK/api/src-client/common/client/renderer/texture/PBRTexturePtrs.h"

namespace mce {
    class TexturePtr;
    class MaterialPtr;
} // namespace mce

// size: 200
class ParticleRenderData {
public:
    enum class FaceCameraMode : int32_t {
        None = 0,
        RotateXYZ = 1,
        RotateY = 2,
        LookatXYZ = 3,
        LookatY = 4,
        LookatDirection = 5,
        DirectionX = 6,
        DirectionY = 7,
        DirectionZ = 8,
        EmitterTransformXY = 9,
        EmitterTransformXZ = 10,
        EmitterTransformYZ = 11,
    };

    // size: 164
    struct ParticleData {
        mce::Color     mColor;             // off+0
        __int8         mUnk16[2];          // off+16
        Vec2           mUV0;               // off+20
        Vec2           mUV1;               // off+28
        Vec3           mWorldPos;          // off+36
        Vec3           mParticleDirection; // off+48
        Vec2           mSize;              // off+60
        Vec4           mRandomNumbers;     // off+68
        float          mRotation;          // off+84
        uint16_t       mTextureIdx;        // off+88
        uint16_t       mMaterialIdx;       // off+90
        FaceCameraMode mFaceCameraMode;    // off+92
        Matrix         mCustomOrientation; // off+96
        __int32        mUnk160;            // off+160
    };

    std::vector<ParticleData>                  mParticles;     // off+0
    std::vector<PBRTexturePtrs>                mTexturesList;  // off+24
    std::vector<mce::MaterialPtr>              mMaterialsList; // off+48
    std::unordered_map<HashedString, uint16_t> mTexturesMap;   // off+72
    std::unordered_map<HashedString, uint16_t> mMaterialsMap;  // off+136
};