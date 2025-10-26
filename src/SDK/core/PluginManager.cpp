#include "PluginManager.h"

#include "SDK/api/sapphire/hook/Hook.h"
#include "SDK/api/sapphire/logger/Logger.h"
#include "SDK/api/src-client/common/client/game/MinecraftGame.h"

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
        PluginManager::OnMinecraftGameLeaveGameHook,
        MinecraftGame,
        HookPriority::Normal,
        MinecraftGame::requestLeaveGame,
        void,
        bool switchScreen,
        bool sync
    ) {
        gPluginManager->_onUnInitPlugins();
        this->origin(switchScreen, sync);
    }

    void sapphire::PluginManager::pluginsOnLoaded() {
        for (auto &&plg : this->mLoadedPlugins) {
            plg.instance->onLoaded();
            plg.loaded = true;
        }
        gPluginManager = this;
        if (!OnMinecraftGameInitCompleteHook::hook())
            Logger::Error("[PluginManager] failed to install OnMinecraftGameInitCompleteHook");
        if (!OnMinecraftGameLeaveGameHook::hook())
            Logger::Error("[PluginManager] failed to install OnMinecraftGameLeaveGameHook");
    }

    void sapphire::PluginManager::unloadAllPlugins() {
        OnMinecraftGameLeaveGameHook::unhook();
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
