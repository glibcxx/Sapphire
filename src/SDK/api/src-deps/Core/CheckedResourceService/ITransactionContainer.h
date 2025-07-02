#pragma once

namespace mce {

    class IDeferredDebugUpdate {
    public:
        // vtb+0
        virtual ~IDeferredDebugUpdate() = 0;

        // vtb+1
        virtual void apply() = 0;
    };

    // size: 8
    class ITransactionContainer {
    public:
        // vtb+0
        virtual ~ITransactionContainer() = 0;

        // vtb+1
        virtual std::unique_ptr<IDeferredDebugUpdate> apply() = 0;
    };

} // namespace mce