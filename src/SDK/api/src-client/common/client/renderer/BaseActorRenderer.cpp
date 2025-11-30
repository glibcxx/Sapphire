#include "BaseActorRenderer.h"
#include "SDK/core/SymbolResolver.h"

const mce::Color *BaseActorRenderer::NAME_TAG_COLOR = &sapphire::bind::data<
#if MC_VERSION == v1_21_2
    sapphire::deRefMov | "\x0F\x10\x05\x00\x00\x00\x00\x0F\x28\x25"_sig,
#elif MC_VERSION == v1_21_50 || MC_VERSION == v1_21_60
    sapphire::deRefMov | "\x0F\x10\x1D\x00\x00\x00\x00\x0F\x29\x5D"_sig,
#endif
    &BaseActorRenderer::NAME_TAG_COLOR,
    mce::Color>();