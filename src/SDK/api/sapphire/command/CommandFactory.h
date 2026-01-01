#pragma once

#include "SDK/api/src/common/server/commands/Command.h"
#include <cstddef>
#include <memory>
#include <utility>

namespace sapphire::command {

    template <typename TOverloaded, typename TCommand, auto Seed = []() {}>
    class CommandFactory {
        alignas(TOverloaded) inline static std::byte sOverloaded[sizeof(TOverloaded)];

        template <typename... Args>
        static TOverloaded &setOverload(Args &&...args) {
            return *new (&sOverloaded) TOverloaded{std::forward<Args>(args)...};
        }

        static TOverloaded &getOverload() {
            return *std::launder(reinterpret_cast<TOverloaded *>(&sOverloaded));
        }

        static std::unique_ptr<::Command> make() {
            return std::make_unique<TCommand>(getOverload());
        }

    public:
        template <typename... Args>
        static auto factory(Args &&...args) {
            setOverload(std::forward<Args>(args)...);
            return &make;
        }
    };

} // namespace sapphire::command