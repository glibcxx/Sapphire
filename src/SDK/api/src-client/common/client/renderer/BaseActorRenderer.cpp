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

std::vector<NameTagRenderObject> BaseActorRenderer::extractRenderTextObjects(
    Tessellator            &tessellator,
    const std::string      &str,
    const std::vector<int> &widths,
    const Vec3             &pos,
    const mce::Color       &color,
    bool                    realityFullVRMode
) {
    using Bind = sapphire::bind::Fn<
        sapphire::deRefCall | "\xE8\x00\x00\x00\x00\x48\x8B\xC3\x48\x83\xC4\x00\x5B\xC3\x4C\x89\x53"_sig,
        &BaseActorRenderer::extractRenderTextObjects>;
    return (this->*Bind::origin)(tessellator, str, widths, pos, color, realityFullVRMode);
}

void BaseActorRenderer::renderText(
    ScreenContext             &screenContext,
    const ViewRenderData      &viewData,
    const NameTagRenderObject &tagData,
    Font                      &font,
    float                      size
) {
    using Bind = sapphire::bind::Fn<
        sapphire::deRefCall | "\xE8\x00\x00\x00\x00\x48\x83\xC3\x70\x48\x3B\x00\x75\xD3"_sig,
        &BaseActorRenderer::renderText>;
    Bind::origin(screenContext, viewData, tagData, font, size);
}