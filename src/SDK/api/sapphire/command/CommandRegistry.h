#pragma once

#include "SDK/api/src-deps/Core/Utility/NonOwnerPointer.h"
#include "SDK/api/src/common/server/commands/CommandRegistry.h"
#include "common/reflection/Reflection.hpp"
#include "CommandFactory.h"
#include "CommandParam.h"
#include "Command.h"

class Minecraft;

namespace sapphire::command {

    class CommandRegistryM {
        CommandRegistry                *mRegistry = nullptr;
        std::unordered_set<std::string> mCommandTextStorage;

        CommandRegistryM() = default;

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
            mRegistry->addEnumValues<E>(std::string{refl::typeName<E>}, evParams);
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
            mRegistry.mRegistry->registerOverloadInternal(mSig, overload);
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
                            mRegistry.mRegistry->addEnumValues<TextPlaceholder>(
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
                mRegistry.mRegistry->registerOverloadInternal(mSig, overload);
            }(),
             ...);
        }
    };

    inline CommandHandle CommandRegistryM::getOrCreateCommand(
        const std::string     &name,
        const std::string     &description,
        CommandPermissionLevel requirement
    ) {
        auto *sig = mRegistry->findCommand(name);
        if (!sig) {
            mRegistry->registerCommand(name, description.data(), requirement);
            sig = mRegistry->findCommand(name);
            assert(sig);
        }
        return {*sig, *this};
    }

} // namespace sapphire::command