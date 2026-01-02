#pragma once

namespace mce {

    // https://github.com/LiteLDev/LeviLamina/blob/dad11c69b20f644ab7192d45add4e0be1d54d432/src-client/mc/deps/renderer/hal/enums/RenderFeature.h#L7

    enum class RenderFeature : uint8_t {
        TexelAA = 0,
        Msaa = 1,
        AlphaToCoverage = 2,
        MipMapMaxLevel = 3,
        ImmutableTextures = 4,
        DynamicShaderLoops = 5,
        VertexFormatRGBA8 = 6,
        VertexFormatR16 = 7,
        Tearing = 8,
        HalfFloats = 9,
        VertexShaderInstancing = 10,
        ShaderBinary = 11,
        SRGBTexturesAndRenderTargets = 12,
        RGB10TextureSupported = 13,
        R8TextureSupported = 14,
        LargeVertexShaderUniforms = 15,
        PackedDepthStencilFrameBufferStorage = 16,
        StandardDerivatives = 17,
        DetachShadersAfterLink = 18,
        Blit = 19,
        FeatureCount = 20,
    };

} // namespace mce