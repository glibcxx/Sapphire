#include "CommandRegistry.h"
#include "SDK/api/sapphire/service/Service.h"
#include "SDK/api/src-client/common/client/game/ClientInstance.h"
#include "SDK/api/src/common/server/ServerInstance.h"
#include "SDK/api/src/common/server/commands/MinecraftCommands.h"

namespace sapphire::command {

    CommandRegistryM &CommandRegistryM::client() {
        static CommandRegistryM clientRegistry{};
        auto                   *client = sapphire::getPrimaryClientInstance();
        assert(client);
        assert(client->mMinecraft);
        auto &mc = *client->mMinecraft;
        assert(mc.mCommands);
        clientRegistry.mRegistry = mc.mCommands->mRegistry.get();
        return clientRegistry;
    }

    CommandRegistryM &CommandRegistryM::server() {
        static CommandRegistryM serverRegistry{};
        auto                   *server = sapphire::getServerInstance();
        assert(server);
        assert(server->mMinecraft);
        auto &mc = *server->mMinecraft;
        assert(mc.mCommands);
        serverRegistry.mRegistry = mc.mCommands->mRegistry.get();
        return serverRegistry;
    }

} // namespace sapphire::command