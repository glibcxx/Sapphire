#pragma once

namespace dragon::mesh {

    enum class VertexField : uint16_t {};

    enum class VertexFieldType : int32_t {};

    // size: 184 (1.21.50)
    struct VertexFormat {
        struct FieldEntry // size: 12
        {
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