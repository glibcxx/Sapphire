#pragma once

#include <vector>
#include <optional>
#include <system_error>
#include "SDK/api/src-deps/Core/Debug/BedrockEventLoggerAreas.h"

namespace Bedrock {

    enum class LogLevel : int {
        Verbose = 0,
        Info = 1,
        Warning = 2,
        Error = 4,
    };

    struct CallStack {
        struct Frame {
            size_t           mFilenameHash;
            std::string_view mFilename;
            uint32_t         mLine;
        };

        struct Context {
            std::string              mValue;
            std::optional<LogLevel>  mLogLevel;
            std::optional<LogAreaID> mLogArea;
        };

        struct FrameWithContext {
            Bedrock::CallStack::Frame                  mFrame;
            std::optional<Bedrock::CallStack::Context> mContext;
        };
        std::vector<FrameWithContext> mFrames;
    };

    template <typename E = std::error_code>
    class ErrorInfo {
    public:
        E                      mError;
        CallStack              mCallStack;
        std::vector<ErrorInfo> mStackErrors;
    };

} // namespace Bedrock