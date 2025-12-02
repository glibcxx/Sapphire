#include "PluginManager.h"

#include "SDK/api/sapphire/hook/Hook.h"
#include "SDK/api/sapphire/logger/Logger.h"
#include "SDK/api/sapphire/platform/Environment.h"
#include "SDK/api/src-client/common/client/game/MinecraftGame.h"
#include "SDK/api/src-client/common/client/game/ClientInstance.h"
#include "Runtime.h"
#include "IatPatcher.h"

namespace fs = std::filesystem;

namespace sapphire::core {

    static PluginManager *gPluginManager = nullptr;

    HOOK_TYPE(
        PluginManager::OnMinecraftGameInitCompleteHook,
        MinecraftGame,
        HookPriority::Normal,
        MinecraftGame::_initFinish,
        SerialWorkList::WorkResult,
        std::shared_ptr<MinecraftGame::InitContext> &initContext
    ) {
        auto res = this->origin(initContext);
        if (res == SerialWorkList::WorkResult::Complete) {
            PluginInitContext ctx{
                *this,
                *initContext->mIncompletePrimaryClient
            };
            gPluginManager->_onInitPlugins(ctx);
        }
        return res;
    }

    HOOK_TYPE(
        PluginManager::OnDestroyMinecraftGameHook,
        ClientInstance,
        HookPriority::Normal,
        ClientInstance::onDestroyMinecraftGame,
        void
    ) {
        gPluginManager->_onUnInitPlugins();
        this->origin();
    }

    bool PluginManager::loadAllPlugins() {
        DWORD    dwProcessId;
        fs::path modsDir = platform::Environment::getInstance().getSapphireHomePath() / "mods";
        if (!fs::exists(modsDir)) {
            fs::create_directory(modsDir);
        }
        if (!fs::is_directory(modsDir)) {
            sapphire::error("PluginManager: 未找到插件目录！{}", modsDir.string());
            return false;
        }

        auto      &apiMap = SymbolResolver::getInstance().getFunctionApiMap();
        IatPatcher iatPatcher{platform::Environment::getInstance().getSapphireCorePath().filename().string()};
        size_t     loaded = mLoadedPlugins.size();
        for (auto &&entry : fs::directory_iterator{modsDir}) {
            auto filename = entry.path().filename().string();
            if (!entry.is_regular_file() || entry.path().extension() != ".dll")
                continue;
            sapphire::info("PluginManager: 注入文件 {}...", filename);
            HMODULE handle = LoadLibrary(entry.path().c_str());
            if (!handle) {
                sapphire::error("PluginManager: {} 注入失败！", filename);
                continue;
            }
            size_t current = mLoadedPlugins.size();
            if (current > loaded + 1) {
                sapphire::error(
                    "PluginManager: 不得在一个dll插件注入期间注入另一个dll插件或者一个dll创建多个IPlugin实例！({})",
                    filename
                );
                mLoadedPlugins.erase(
                    mLoadedPlugins.begin() + loaded,
                    mLoadedPlugins.end()
                );
            } else if (current == loaded) {
                sapphire::warn(
                    "PluginManager: {} 未向sapphire注册，可能不是标准sapphire插件，sapphire将无法管理其生命周期！",
                    filename
                );
            } else if (current < loaded) {
                sapphire::error("PluginManager: 加载 {} 后出现意外数据丢失，终止进程！", filename);
                return false;
            } else {
                mLoadedPlugins.back().handle = handle;
                loaded++;
                iatPatcher.patchModule(handle, apiMap);
                sapphire::info("PluginManager: {} 注入成功！", filename);
            }
        }
        return true;
    }

    void PluginManager::pluginsOnLoaded() {
        for (auto &&plg : this->mLoadedPlugins) {
            plg.instance->onLoaded();
            plg.loaded = true;
        }
        gPluginManager = this;
        if (!OnMinecraftGameInitCompleteHook::hook())
            sapphire::error("PluginManager: failed to install OnMinecraftGameInitCompleteHook");
        if (!OnDestroyMinecraftGameHook::hook())
            sapphire::error("PluginManager: failed to install OnDestroyMinecraftGameHook");
    }

    void PluginManager::unloadAllPlugins() {
        OnDestroyMinecraftGameHook::unhook();
        OnMinecraftGameInitCompleteHook::unhook();
        gPluginManager = nullptr;
        this->mLoadedPlugins.clear();
    }

    void PluginManager::_onInitPlugins(const PluginInitContext &ctx) {
        for (auto &&plg : this->mLoadedPlugins) {
            if (plg.loaded && !plg.initialized) {
                plg.instance->onInit(ctx);
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

} // namespace sapphire::core
