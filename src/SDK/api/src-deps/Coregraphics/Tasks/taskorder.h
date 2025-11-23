#pragma once

namespace cg::taskorder {

    template <typename ExecutionContext>
    class TaskBuilder {
    public:
        std::string mName; // off+0
        using ActionFunction = std::function<void(const ExecutionContext &)>;
    };

    template <typename ExecutionContext, typename Context = void>
    class TaskBuilderWithAction : public TaskBuilder<ExecutionContext> {
    public:
        Context                                                mContext; // off+32
        typename TaskBuilder<ExecutionContext>::ActionFunction mAction;
    };

    template <typename ExecutionContext>
    class TaskBuilderWithAction<ExecutionContext, void> : public TaskBuilder<ExecutionContext> {
    public:
        typename TaskBuilder<ExecutionContext>::ActionFunction mAction; // off+32
    };

} // namespace cg::taskorder