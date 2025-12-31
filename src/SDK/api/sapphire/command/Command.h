#pragma once

#include "SDK/api/src/common/server/commands/Command.h"
#include "SDK/api/src/common/server/commands/CommandRegistry.h"
#include "SDK/api/src/common/server/commands/CommandPermissionLevel.h"
#include "common/reflection/Reflection.hpp"
#include "common/reflection/TypeInfo.hpp"
#include <cstddef>
#include <functional>
#include <memory>
#include <new>
#include <type_traits>
#include <unordered_map>
#include <utility>

class CommandOutput;
class CommandOrigin;

namespace sapphire::command {

    struct text {};

    enum class TextPlaceholder : uint8_t {};

#define SPHR_CMD_TEXT(name) \
    [[msvc::no_unique_address]] ::sapphire::command::text name

    template <typename T>
    struct opt {
        using Type = T;
        T mValue;

        constexpr opt &operator=(const T &value) noexcept(std::is_nothrow_copy_assignable_v<T>) {
            mValue = value;
            return *this;
        }
        constexpr opt &operator=(T &&value) noexcept(std::is_nothrow_move_assignable_v<T>) {
            mValue = std::move(value);
            return *this;
        }
        constexpr operator T &() noexcept { return mValue; }
        constexpr operator const T &() const noexcept { return mValue; }

        constexpr T       &value() noexcept { return mValue; }
        constexpr const T &value() const noexcept { return mValue; }
        constexpr T       &operator*() noexcept { return mValue; }
        constexpr const T &operator*() const noexcept { return mValue; }
        constexpr T       *operator->() noexcept { return &mValue; }
        constexpr const T *operator->() const noexcept { return &mValue; }
    };

    template <typename T>
    constexpr bool isOpt = false;

    template <typename T>
    constexpr bool isOpt<opt<T>> = true;

    template <typename T>
    struct RemoveOpt {
        using Type = T;
    };

    template <typename T>
    struct RemoveOpt<opt<T>> {
        using Type = T;
    };

    template <typename T>
    using RemoveOptT = RemoveOpt<T>::Type;

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

    class CommandHandle;

    class CommandRegistryM {
        CommandRegistry                &mRegistry;
        std::unordered_set<std::string> mCommandTextStorage;

        CommandRegistryM(CommandRegistry &registry) :
            mRegistry(registry) {}

        friend class CommandHandle;

    public:
        SPHR_API static CommandRegistryM &client();
        SPHR_API static CommandRegistryM &server();

        CommandHandle getOrCreateCommand(
            const std::string     &name,
            const std::string     &description,
            CommandPermissionLevel requirement = CommandPermissionLevel::Any
        );

    private:
        template <typename E>
            requires std::is_enum_v<E>
        void registerEnum() const {
            std::vector<std::pair<std::string, E>> evParams;
            evParams.reserve(refl::enumNames<E>.size());
            for (auto [value, name] : refl::enumNames<E>) {
                evParams.emplace_back(name, value);
            }
            mRegistry.addEnumValues<E>(std::string{refl::typeName<E>}, evParams);
        }
    };

    class CommandHandle {
        CommandRegistry::Signature &mSig;
        CommandRegistryM           &mRegistry;

    public:
        CommandHandle(CommandRegistry::Signature &sig, CommandRegistryM &registry) :
            mSig(sig), mRegistry(registry) {}

        template <typename... Execute>
        void overloads(Execute &&...exe) {
            using TOverloaded = util::Overloaded<Execute...>;
            using TCommand = Command<TOverloaded, void>;
            using TCommandFactory = CommandFactory<util::Overloaded<Execute...>, TCommand>;
            TCommandFactory::sOverloaded.emplace(std::forward<Execute>(exe)...);
            auto &overload = mSig.overloads.emplace_back(
                CommandVersion{}, &TCommandFactory::factory(std::forward<Execute>(exe)...)
            );
            mRegistry.mRegistry.registerOverloadInternal(mSig, overload);
        }

        template <typename... ParamStorages, typename... Execute>
            requires(sizeof...(ParamStorages) > 0 && (refl::isMembReflectable<ParamStorages> && ...))
        void overloads(Execute &&...exe) {
            mSig.overloads.reserve(sizeof...(ParamStorages) + mSig.overloads.size());
            ([&]() {
                using TOverloaded = util::Overloaded<Execute...>;
                using TCommand = Command<TOverloaded, ParamStorages>;
                using TCommandFactory = CommandFactory<TOverloaded, TCommand>;
                auto &overload = mSig.overloads.emplace_back(
                    CommandVersion{}, TCommandFactory::factory(std::forward<Execute>(exe)...)
                );
                overload.params.reserve(refl::membCount<ParamStorages>);
                refl::forEachMemberT<ParamStorages>(
                    [&]<typename Param>(std::type_identity<Param>, std::size_t idx, std::string_view name) {
                        using NonOptParam = RemoveOptT<Param>;
                        if constexpr (std::is_enum_v<NonOptParam>) {
                            mRegistry.registerEnum<NonOptParam>();
                        }
                        if constexpr (std::is_same_v<NonOptParam, text>) {
                            auto &ph = *mRegistry.mCommandTextStorage
                                            .emplace(std::format("sapphire.cmd.text_placeholder.{}.{}", name, idx))
                                            .first;
                            mRegistry.mRegistry.addEnumValues<TextPlaceholder>(
                                ph,
                                {
                                    {std::string{name}, TextPlaceholder{0}}
                            }
                            );
                            overload.params.emplace_back(
                                type_id<CommandRegistry, TextPlaceholder>(),
                                &CommandRegistry::parse<TextPlaceholder>,
                                std::string{name},
                                CommandParameterDataType::Enum,
                                ph.data(), /*enumNameOrPostfix*/
                                nullptr,   /*subChain*/
                                TCommand::template OFFSET_TEXTPLACEHOLDER<>,
                                false, /*optional*/
                                -1,    /*setOffset*/
                                CommandParameterOption::EnumAutocompleteExpansion
                            );
                        } else if constexpr (std::is_enum_v<NonOptParam>) {
                            overload.params.emplace_back(
                                type_id<CommandRegistry, NonOptParam>(),
                                &CommandRegistry::parseEnum<NonOptParam>,
                                std::string{name},
                                CommandParameterDataType::Enum,
                                refl::typeName<NonOptParam>.data(), /*enumNameOrPostfix*/
                                nullptr,                            /*subChain*/
                                TCommand::template OFFSET_PARAM<> + (int)refl::membOffsets<ParamStorages>[idx],
                                isOpt<NonOptParam>, /*optional*/
                                -1,                 /*setOffset*/
                                CommandParameterOption::None
                            );
                        } else {
                            overload.params.emplace_back(
                                type_id<::CommandRegistry, NonOptParam>(),
                                &::CommandRegistry::parse<NonOptParam>,
                                std::string{name},
                                CommandParameterDataType::Basic,
                                nullptr, /*enumNameOrPostfix*/
                                nullptr, /*subChain*/
                                TCommand::template OFFSET_PARAM<> + (int)refl::membOffsets<ParamStorages>[idx],
                                isOpt<Param>, /*optional*/
                                -1,           /*setOffset*/
                                CommandParameterOption::None
                            );
                        }
                    }
                );
                mRegistry.mRegistry.registerOverloadInternal(mSig, overload);
            }(),
             ...);
        }
    };

    inline CommandHandle CommandRegistryM::getOrCreateCommand(
        const std::string     &name,
        const std::string     &description,
        CommandPermissionLevel requirement
    ) {
        auto *sig = mRegistry.findCommand(name);
        if (!sig) {
            mRegistry.registerCommand(name, description.data(), requirement);
            sig = mRegistry.findCommand(name);
            assert(sig);
        }
        return {*sig, *this};
    }

} // namespace sapphire::command
