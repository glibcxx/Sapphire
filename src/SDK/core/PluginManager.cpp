#include "PluginManager.h"

#include "SDK/api/sapphire/hook/Hook.h"
#include "SDK/api/sapphire/logger/Logger.h"
#include "SDK/api/src-client/common/client/game/MinecraftGame.h"
#include "SDK/api/src-client/common/client/game/ClientInstance.h"

namespace sapphire {

    static PluginManager *gPluginManager = nullptr;

    INDIRECT_HOOK_TYPE(
        PluginManager::OnMinecraftGameInitCompleteHook,
        MinecraftGame,
        HookPriority::Normal,
        MinecraftGame::_initFinish,
        SerialWorkList::WorkResult,
        std::shared_ptr<MinecraftGame::InitContext> &initContext
    ) {
        auto res = this->origin(initContext);
        if (res == SerialWorkList::WorkResult::Complete) {
            gPluginManager->_onInitPlugins();
        }
        return res;
    }

    INDIRECT_HOOK_TYPE(
        PluginManager::OnDestroyMinecraftGameHook,
        ClientInstance,
        HookPriority::Normal,
        ClientInstance::onDestroyMinecraftGame,
        void
    ) {
        gPluginManager->_onUnInitPlugins();
        this->origin();
    }

    void sapphire::PluginManager::pluginsOnLoaded() {
        for (auto &&plg : this->mLoadedPlugins) {
            plg.instance->onLoaded();
            plg.loaded = true;
        }
        gPluginManager = this;
        if (!OnMinecraftGameInitCompleteHook::hook())
            Logger::Error("[PluginManager] failed to install OnMinecraftGameInitCompleteHook");
        if (!OnDestroyMinecraftGameHook::hook())
            Logger::Error("[PluginManager] failed to install OnDestroyMinecraftGameHook");
    }

    void sapphire::PluginManager::unloadAllPlugins() {
        OnDestroyMinecraftGameHook::unhook();
        OnMinecraftGameInitCompleteHook::unhook();
        gPluginManager = nullptr;
        this->mLoadedPlugins.clear();
    }

    void PluginManager::_onInitPlugins() {
        for (auto &&plg : this->mLoadedPlugins) {
            if (plg.loaded && !plg.initialized) {
                plg.instance->onInit();
                plg.initialized = true;
            }
        }
    }

    void PluginManager::_onUnInitPlugins() {
        for (auto &&plg : this->mLoadedPlugins) {
            if (plg.loaded && plg.initialized) {
                plg.initialized = false;
                plg.instance->onUnInit();
            }
        }
    }

} // namespace sapphire
