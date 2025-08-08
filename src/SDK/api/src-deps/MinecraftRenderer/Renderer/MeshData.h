#pragma once

#include <glm/glm.hpp>
#include "SDK/core/ApiManager.h"
#include "SDK/api/src-deps/Core/Utility/PrimitiveTypes.h"

namespace mce {

    // size: 264 (1.21.2), 288 (1.21.50)
    class MeshData {
    public:
        using Index = uint32_t;

        PrimitiveMode                  mMode;              // off+0
        bool                           mMeshNotFree;       // off+1
        std::vector<glm::tvec3<float>> mPositions;         // off+8
        std::vector<glm::tvec4<float>> mNormals;           // off+32
        std::vector<glm::tvec4<float>> mUnk56;             // off+56
        std::vector<uint32_t>          mIndices;           // off+80
        std::vector<uint32_t>          mColors;            // off+104
        std::vector<uint16_t>          mBoneId0s;          // off+128
        std::vector<glm::tvec2<float>> mTextureUVs[3];     // off+152
        std::vector<uint16_t>          mPBRTextureIndices; // off+224
#if MC_VERSION >= v1_21_50
        std::vector<uint32_t> mUnk248; // off+248
#endif
        std::array<bool, 14> mFieldEnabled;

        MeshData *ctor(); // \xE8\x00\x00\x00\x00\x48\x89\xB3\x00\x00\x00\x00\x48\x89\xB3\x00\x00\x00\x00\x48\x89\xBB

        MeshData *ctor(MeshData &&other); // \xE8\x00\x00\x00\x00\x90\x48\x89\xAB\x00\x00\x00\x00\x48\x89\xAB\x00\x00\x00\x00\x4C\x89\xB3 1.21.50

        SDK_API void dtor();
        MARK_HOOKABLE(&MeshData::dtor)

#if MC_VERSION == v1_21_50 || MC_VERSION == v1_21_60
        SDK_API MeshData clone() const;
        MARK_HOOKABLE(&MeshData::clone)
#endif

        SDK_API void clear();
        MARK_HOOKABLE(&MeshData::clear)
    };
#if MC_VERSION == v1_21_2
    static_assert(sizeof(MeshData) == 264);
#elif MC_VERSION == v1_21_50
    static_assert(sizeof(MeshData) == 288);
#endif

} // namespace mce