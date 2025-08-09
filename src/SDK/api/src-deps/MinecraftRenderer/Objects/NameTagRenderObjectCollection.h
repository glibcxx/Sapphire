#pragma once

#include "SDK/api/src-client/common/client/renderer/MaterialPtr.h"
#include "SDK/api/src-deps/MinecraftRenderer/Renderer/Mesh.h"
#include "SDK/api/src-deps/Core/Math/Color.h"

// size: 112
struct NameTagRenderObject {
    std::string                mNameTag;         // off+0
    std::shared_ptr<mce::Mesh> mMesh;            // off+32
    mce::MaterialPtr          *mTagMat;          // off+48
    mce::MaterialPtr          *mTextMatOverride; // off+56
    mce::Color                 mTagColor;        // off+64
    mce::Color                 mTextColor;       // off+80
    glm::tvec3<float>          mPos;             // off+96
};

// size: 40
struct NameTagRenderObjectCollection {
    std::vector<NameTagRenderObject> mNameTags; // off+0
};