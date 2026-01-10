#pragma once

#include "SDK/api/src-deps/MinecraftRenderer/renderer/TexturePtr.h"

// https://github.com/LiteLDev/LeviLamina/blob/4d08de07ef76fb4f101aebb8b251e2652dc4cfbd/src/mc/deps/minecraft_renderer/resources/PBRTexturePtrs.h#L5

// size: 112
class PBRTexturePtrs {
public:
    struct MERSTexturePtr {
        mce::TexturePtr mTexture;
        bool            mValid;
    };

    struct NormalTexturePtr {
        mce::TexturePtr mTexture;
        bool            mValid;
    };

    mce::TexturePtr  mTexture;
    MERSTexturePtr   mMERSTexture;
    NormalTexturePtr mNormalTexture;
};