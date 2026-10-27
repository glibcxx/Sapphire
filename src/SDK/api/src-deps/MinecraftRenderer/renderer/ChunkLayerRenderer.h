#pragma once

#include "SDK/api/src-deps/Core/Utility/NonOwnerPointer.h"
#include "SDK/api/src-deps/Renderer/constantbuffers/AlignmentHelper.h"

class CommandListQueue;
class TerrainLayer;
struct ViewRenderData;
struct ChunkRenderObjectCollection;
enum CloudRenderSideRelative : int;
namespace mce::framebuilder {
    class FrameBuilder;
}
namespace DFC {
    struct ViewSetId;
}

namespace Renderers {

    SPHR_DECL_API("1.21.50", "disp:15,call", "\x4C\x8D\x43\x00\x48\x8D\x53\x00\x48\x8D\x8D\x00\x00\x00\x00\xE8\x00\x00\x00\x00\x90\xBF")
    SDK_API void _insertChunkLayer(
        const Bedrock::NonOwnerPointer<mce::framebuilder::FrameBuilder>                   &frameBuilder,
        const std::vector<uint8_t, mce::AlignmentHelper::AlignmentAllocator<uint8_t, 16>> &perFrameConstantsMemory,
        const DFC::ViewSetId                                                              &viewId,
        CommandListQueue                                                                  &commandListQueue,
        const ViewRenderData                                                              &viewData,
        std::shared_ptr<const ChunkRenderObjectCollection>                                 chunkState,
        const TerrainLayer                                                                &layer,
        const float                                                                        renderchunkFadeInTime,
        CloudRenderSideRelative                                                            cloudSide
    );

} // namespace Renderers