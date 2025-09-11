#pragma once

namespace dragon::mesh {

    enum class VertexField : uint16_t {
        Position = 0,               // 0
        Color = 1,                  // 4
        Normal = 2,                 // 1
        UV0 = 3,                    // 2
        UV1 = 4,                    // 10
        UV2 = 5,                    // 11
        PBRTextureIdx = 6,          // 12
        BoneId0 = 7,                // 14
        PreviousPosition = 8,       // 8
        HummingbirdPosition = 9,    // 15
        HummingbirdColor = 10,      // 0
        HummingbirdAdditional = 11, // 4
        UnkownEnum12 = 12,          // 13
        _count = 13,                // 18
        InvalidField = 65535,
    };

    enum class VertexFieldType : int32_t {
        Unknown = 0, //
        SInt8 = 1,   //
        UInt8 = 2,   //
        SInt16 = 3,  //
        UInt16 = 4,  //
        SInt32 = 5,  //
        UInt32 = 6,  //
        Half = 7,    //
        Float = 8,   //
    };

    // size: 184 (1.21.50)
    struct VertexFormat {
        // size: 12
        struct FieldEntry {
            VertexField     mField;  // off+0
            VertexFieldType mType;   // off+4
            uint16_t        mOffset; // off+8
            uint8_t         mCount;  // off+10
        };

        struct boost__container__static_vector__dragon__mesh__VertexFormat__FieldEntry__14_ {
            FieldEntry storage[14]; // off+0
            uint64_t   m_size;      // off+168
        };

        boost__container__static_vector__dragon__mesh__VertexFormat__FieldEntry__14_ mFields; // off+0

        uint16_t mStride; // off+176
    };

} // namespace dragon::mesh