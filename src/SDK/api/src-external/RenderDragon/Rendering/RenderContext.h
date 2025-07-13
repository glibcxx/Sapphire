#pragma once

#include <memory>

namespace dragon::rendering {

    class CommandContext;

    // size: 1
    class RenderContext {
        using CommandContext = CommandContext;
        using SharedCommandContext = std::shared_ptr<CommandContext>;
    };

} // namespace dragon::rendering