#pragma once

#include <optional>

namespace dragon::materials::definition {

    // size: 1
    struct ExternalUniformDeclaration {};

    enum class InputSemantic : uint8_t {
        Position = 0,
        Normal = 1,
        Tangent = 2,
        Bitangent = 3,
        Color = 4,
        Indices = 5,
        Weight = 6,
        TexCoord = 7,
    };

    enum class ShaderInputType : int {
        Float = 0,
        Vec2 = 1,
        Vec3 = 2,
        Vec4 = 3,
        Int = 4,
        Int2 = 5,
        Int3 = 6,
        Int4 = 7,
        UInt = 8,
        UInt2 = 9,
        UInt3 = 10,
        UInt4 = 11,
        Mat4 = 12,
    };

    enum class ShaderStage : uint8_t {
        Vertex = 0,
        Fragment = 1,
        Compute = 2,
    };

    enum class SamplerAccess : uint8_t {
        None = 0,
        Read = 1,
        Write = 2,
        ReadWrite = 3,
    };

    enum class ShaderType : int {
        Compute = 0,
        Custom = 1,
        Standard = 2,
    };

    enum class InterpolationConstraint : uint8_t {
        Flat = 0,
        Smooth = 1,
        NoPerspective = 2,
        Centroid = 3,
    };

    enum class PrecisionConstraint : uint8_t {
        Low = 0,
        Medium = 1,
        High = 2,
    };

    enum class SamplerType : uint8_t {
        Type2D = 0,
        Type2DArray = 1,
        Type2DExternal = 2,
        Type3D = 3,
        TypeCube = 4,
        TypeStructuredBuffer = 5,
        TypeRawBuffer = 6,
    };

    enum class BlendMode : int {
        None = 0,
        Replace = 1,
        AlphaBlend = 2,
        ColorBlendAlphaAdd = 3,
        PreMultiplied = 4,
        InvertColor = 5,
        Additive = 6,
        AdditiveAlpha = 7,
        Multiply = 8,
        MultiplyBoth = 9,
        InverseSrcAlpha = 10,
        SrcAlpha = 11,
    };

    template <typename Ty, typename Default = std::optional<Ty>>
    struct PropertyField {
        int     mArrayCount = 0; // off+0
        Default mDefaultValue;   // off+4

        bool mUnkFlagA = false;
        bool mUnkFlagB = false;
        bool mUnkFlagC = false;
    };

    struct ShaderContent {};

    struct MaterialPassDeclaration {};

    struct MaterialPass {
        ShaderContent           mShader;       // off+0
        MaterialPassDeclaration mDeclarations; // off+248
    };

    struct MaterialUsedPass {};

    struct SemanticDescription {
        InputSemantic mInput; // off+0
        uint8_t       mIndex; // off+1
    };

    struct ShaderInput // size: 12
    {
        ShaderInputType                        mType;                    // off+0
        bool                                   mIsPerInstance;           // off+4
        SemanticDescription                    mSemantic;                // off+5
        std::optional<PrecisionConstraint>     mPrecisionConstraint;     // off+7
        std::optional<InterpolationConstraint> mInterpolationConstraint; // off+9
    };

} // namespace dragon::materials::definition