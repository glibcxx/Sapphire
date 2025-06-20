#pragma once

#include <glm/glm.hpp>
#include "SDK/api/src-deps/Core/Utility/PrimitiveTypes.h"

namespace mce {

    // size: 288 (1.21.50)
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
        std::vector<uint32_t>          mUnk248;            // off+248
        std::array<bool, 12>           mFieldEnabled;      // off+272

        MeshData *ctor(MeshData &&other); // \xE8\x00\x00\x00\x00\x90\x48\x89\xAB\x00\x00\x00\x00\x48\x89\xAB\x00\x00\x00\x00\x4C\x89\xB3 1.21.50

        void dtor(); // \xE8\x00\x00\x00\x00\x4C\x8B\x8E\x00\x00\x00\x00\x45\x89\xA1 1.21.50

        MeshData clone() const; // \x48\x89\x5C\x24\x00\x48\x89\x54\x24\x00\x57\x48\x83\xEC\x00\x48\x8B\xFA\x48\x8B\xD9\x0F\xB6\x01

        void clear(); // \x48\x83\xEC\x00\xC6\x01 1.21.50
    };

} // namespace mce