#pragma once

#include <memory>
#include "../Utility/buffer_span.h"

namespace Core {

    class CpuRingBufferAllocation_Buffer {
    public:
        using Ptr = std::shared_ptr<CpuRingBufferAllocation_Buffer>;
        using BufferEntry = buffer_span_mut<unsigned char>;
    };

} // namespace Core