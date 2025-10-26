#pragma once

namespace Bedrock {

    // size: 8
    class LogEndPoint {
    public:
        // vtb+0
        virtual ~LogEndPoint();

        // vtb+1
        virtual void log(const char *) = 0;

        // vtb+2
        virtual void flush() = 0;

        // vtb+3
        virtual void setEnabled(bool) = 0;

        // vtb+4
        virtual bool isEnabled() const = 0;
    };

} // namespace Bedrock
