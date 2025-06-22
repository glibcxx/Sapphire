#pragma once

#include "SpanRegistrySplitMergeBased.h"
#include "ResidentVertexSpan.h"
#include "ResidentIndexSpan.h"
#include "SDK/api/src-external/RenderDragon/Memory/BufferSource.h"

namespace dragon::mesh {

    // size: 448 (1.21.50)
    struct VertexSpanRegistry
        : public SplitMergeBasedSpanRegistry<VertexSpanType, dragon::memory::BufferSource &&> {
    };

    struct IndexSpanRegistry
        : public SplitMergeBasedSpanRegistry<IndexSpanType, dragon::memory::BufferSource &&> {
    };

} // namespace dragon::mesh