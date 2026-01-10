#pragma once

#include <glm/glm.hpp>
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
        std::vector<glm::tvec4<float>> mTangents;          // off+56
        std::vector<uint32_t>          mIndices;           // off+80
        std::vector<uint32_t>          mColors;            // off+104
        std::vector<uint16_t>          mBoneId0s;          // off+128
        std::vector<glm::tvec2<float>> mTextureUVs[3];     // off+152
        std::vector<uint16_t>          mPBRTextureIndices; // off+224
#if MC_VERSION >= v1_21_50
        std::vector<uint32_t> mMERS; // off+248
#endif
        std::array<bool, 14> mFieldEnabled;

        MeshData *ctor(); // \xE8\x00\x00\x00\x00\x48\x89\xB3\x00\x00\x00\x00\x48\x89\xB3\x00\x00\x00\x00\x48\x89\xBB

        MeshData *ctor(MeshData &&other); // \xE8\x00\x00\x00\x00\x90\x48\x89\xAB\x00\x00\x00\x00\x48\x89\xAB\x00\x00\x00\x00\x4C\x89\xB3 1.21.50

        SPHR_DECL_API("1.21.2", "call", "\xE8\x00\x00\x00\x00\x41\xBD\x00\x00\x00\x00\x48\x8B\x8D")
        SPHR_DECL_API("1.21.50,1.21.60", "call", "\xE8\x00\x00\x00\x00\x4C\x8B\x8E\x00\x00\x00\x00\x45\x89")
        SDK_API void dtor();

        SPHR_DECL_API("1.21.50,1.21.60", "call", "\x48\x89\x5C\x24\x00\x48\x89\x54\x24\x00\x57\x48\x83\xEC\x00\x48\x8B\xFA\x48\x8B\xD9\x0F\xB6\x01")
        SDK_API MeshData clone() const;

        SPHR_DECL_API("1.21.2,1.21.50,1.21.60", "\x48\x83\xEC\x00\xC6\x01\x00\x48\x8B\x41")
        SDK_API void clear();
    };
#if MC_VERSION == v1_21_2
    static_assert(sizeof(MeshData) == 264);
#elif MC_VERSION == v1_21_50
    static_assert(sizeof(MeshData) == 288);
#endif

} // namespace mce