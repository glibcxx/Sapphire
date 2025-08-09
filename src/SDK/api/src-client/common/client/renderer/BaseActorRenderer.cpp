#include "BaseActorRenderer.h"

std::vector<NameTagRenderObject> BaseActorRenderer::extractRenderTextObjects(
    Tessellator            &tessellator,
    const std::string      &str,
    const std::vector<int> &widths,
    const Vec3             &pos,
    const mce::Color       &color,
    bool                    realityFullVRMode
) {
    using Hook = sapphire::ApiLoader<
        sapphire::deRefCall | "\xE8\x00\x00\x00\x00\x48\x8B\xC3\x48\x83\xC4\x00\x5B\xC3\x4C\x89\x53"_sig,
        &BaseActorRenderer::extractRenderTextObjects>;
    return (this->*Hook::origin)(tessellator, str, widths, pos, color, realityFullVRMode);
}