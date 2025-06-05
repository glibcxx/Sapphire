#include "PlayerRenderView.h"

void PlayerRenderView::lambda_75d793f24d52434fc09af39da3e425e3::operator()(
    const LegacyPlayerRenderPass &data, rendergraph::RenderContext &renderContext
) const {
    using Hook = core::ApiLoader<
        "\x48\x89\x5C\x24\x00\x48\x89\x74\x24\x00\x55\x57\x41\x56\x48\x8B\xEC\x48\x81\xEC\x00\x00\x00\x00\x48\x8B\x05\x00\x00\x00\x00\x48\x33\xC4\x48\x89\x45\x00\x48\x8B\x02\x4D\x8B\xF0"_sig,
        &PlayerRenderView::lambda_75d793f24d52434fc09af39da3e425e3::operator()>;
    return (this->*Hook::origin)(data, renderContext);
}
