#pragma once

namespace dragon::materials {

    enum class UniformType : uint8_t {
        Sampler = 0,
        Int1 = 1,
        Vec4 = 2,
        Mat3 = 3,
        Mat4 = 4,
    };

    class MaterialUniform {
    public:
        // size: 16
        struct UniformParameter {
            const MaterialUniform *mParameter;   // off+0
            uint16_t               mValueOffset; // off+8
            uint16_t               mValueSize;   // off+10
        };

        // size: 56
        struct TextureParameter {
            const MaterialUniform *mParameter; // off+0
            uint8_t                _fill[48];
        };

        // size: 56
        struct BufferParameter {
            const MaterialUniform *mParameter; // off+0
            uint8_t                _fill[48];
        };

        // size: 24
        struct UnknownParameter {
            uint8_t _fill[24];
        };

        const UniformType mType;     // off+0
        const std::string mName;     // off+8
        const uint64_t    mNameHash; // off+40
        const uint16_t    mCount;    // off+48
    };

} // namespace dragon::materials