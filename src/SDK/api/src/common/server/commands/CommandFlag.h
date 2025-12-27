#pragma once

#include <cstdint>
#include "SDK/api/src-deps/Core/Utility/EnumCast.h"

enum class CommandUsageFlag : uint8_t {
    Normal = 0,
    Test = 1,
};

enum class CommandVisibilityFlag : uint8_t {
    Visible = 0,
    HiddenFromCommandBlockOrigin = 2,
    HiddenFromPlayerOrigin = 4,
    HiddenFromAutomationOrigin = 8,
    Hidden = 6,
    Removed = 14,
};

enum class CommandSyncFlag : uint8_t {
    Synced = 0,
    Local = 16,
};

enum class CommandExecuteFlag : uint8_t {
    Allowed = 0,
    Disallowed = 32,
};

enum class CommandTypeFlag : uint8_t {
    None = 0,
    Message = 64,
};

enum class CommandCheatFlag : uint8_t {
    Cheat = 0,
    NotCheat = 128,
};

// size: 2
struct CommandFlag {
    constexpr CommandFlag(CommandUsageFlag flag_) : flag(enum_cast(flag_)) {}

    constexpr CommandFlag(CommandVisibilityFlag flag_) : flag(enum_cast(flag_)) {}

    constexpr CommandFlag(CommandSyncFlag flag_) : flag(enum_cast(flag_)) {}

    constexpr CommandFlag(CommandExecuteFlag flag_) : flag(enum_cast(flag_)) {}

    constexpr CommandFlag(CommandTypeFlag flag_) : flag(enum_cast(flag_)) {}

    constexpr CommandFlag(CommandCheatFlag flag_) : flag(enum_cast(flag_)) {}

    constexpr CommandFlag() : flag(0) {}

    constexpr CommandFlag operator|(CommandFlag other) const {
        return static_cast<CommandUsageFlag>(flag | other.flag);
    }

    constexpr bool operator&(CommandFlag other) const {
        return flag & other.flag;
    }

    constexpr auto operator<=>(const CommandFlag &other) const = default;

    uint16_t flag; // off+0
};
