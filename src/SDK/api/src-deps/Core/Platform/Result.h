#pragma once

#include "SDK/api/src/common/platform/Result.h"
#include "SDK/api/src-deps/Core/Platform/OSError.h"

namespace Core {

    class Result : public Bedrock::Result<void, Bedrock::OSError> {
    };

} // namespace Core