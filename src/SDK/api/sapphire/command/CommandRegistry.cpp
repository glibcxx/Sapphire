#include "CommandRegistry.h"
#include "SDK/api/src-client/common/client/game/ClientInstance.h"
#include "SDK/api/src/common/server/commands/MinecraftCommands.h"

namespace sapphire::command {

    CommandRegistryM &CommandRegistryM::client() {
        static CommandRegistryM client{
            []() -> CommandRegistry & {
                auto *client = ClientInstance::primaryClientInstance;
                assert(client);
                assert(client->mMinecraft);
                auto &mc = *client->mMinecraft;
                assert(mc.mCommands);
                return *mc.mCommands->mRegistry;
            }()
        };
        return client;
    }

} // namespace sapphire::command