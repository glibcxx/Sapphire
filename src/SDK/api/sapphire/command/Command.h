#pragma once

#include "SDK/api/src/common/server/commands/Command.h"
#include "SDK/api/src/common/server/commands/CommandRegistry.h"
#include "SDK/api/src/common/server/commands/CommandPermissionLevel.h"

class CommandOutput;
class CommandOrigin;

namespace sapphire::command {

    class CommandHandle;

    enum class TextPlaceholder : uint8_t {};

    template <typename Execute, typename Param>
    class Command : public ::Command {
        const Execute  &mE;
        Param           mParam{};
        TextPlaceholder mText{0};

    public:
        Command(const Execute &e) : mE(e) {}

        virtual void execute(const CommandOrigin &origin, CommandOutput &output) const override {
            mE(origin, output, mParam);
        }

        template <typename = void>
        static constexpr int OFFSET_PARAM = offsetof(Command, mParam);
        template <typename = void>
        static constexpr int OFFSET_TEXTPLACEHOLDER = offsetof(Command, mText);
    };

    template <typename Execute>
    class Command<Execute, void> : public ::Command {
        const Execute &mE;

    public:
        Command(const Execute &e) : mE(e) {}

        virtual void execute(const CommandOrigin &origin, CommandOutput &output) const override {
            mE(origin, output);
        }
    };

} // namespace sapphire::command
