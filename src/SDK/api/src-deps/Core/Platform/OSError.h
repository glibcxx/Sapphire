#pragma once

namespace Bedrock {

    class OSError {
    public:
        std::error_code mCode;
        int             mUnk16;
    };

} // namespace Bedrock