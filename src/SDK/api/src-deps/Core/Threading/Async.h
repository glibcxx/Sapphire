#pragma once
namespace Bedrock::Threading {

    enum AsyncStatus {
        Started = 0,
        Completed = 1,
        Canceled = 2,
        Error = 3,
    };

    enum class AsyncErrc : int {
        operation_in_progress = 1,
        operation_canceled = 2,
        operation_threw_exception = 3,
        unable_to_execute = 4,
        unexpected_error = 5,
        invalid_state = 6,
    };

    // size: 16
    class AsyncBase : public std::enable_shared_from_this<Bedrock::Threading::AsyncBase> {
    };

    // size: 24
    template <typename TResult = void>
    class IAsyncResult : public Bedrock::Threading::AsyncBase {
    public:
        using Handle = std::shared_ptr<IAsyncResult>;
        using CompletionHandler = std::function<void(const IAsyncResult &)>;

        // vtb+0
        virtual ~IAsyncResult();
    };

} // namespace Bedrock::Threading