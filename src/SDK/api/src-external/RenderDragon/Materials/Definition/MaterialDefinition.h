#pragma once

namespace dragon::materials::definition {

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

    struct SamplerDefinition {};
    struct CustomTypeDeclaration {};

} // namespace dragon::materials::definition