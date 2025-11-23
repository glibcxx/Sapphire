#pragma once

#include <minwindef.h>
#include <vector>
#include <ranges>
#include "SDK/api/sapphire/IPlugin.h"

namespace sapphire::core {

    class PluginManager {
        class OnMinecraftGameInitCompleteHook;
        class OnDestroyMinecraftGameHook;

        class LoadedPlugin {
        public:
            HMODULE  handle;
            IPlugin *instance;
            bool     loaded = false;
            bool     initialized = false;

            LoadedPlugin(HMODULE handle, IPlugin *instance) :
                handle(handle), instance(instance) {}

            ~LoadedPlugin() {
                if (loaded) instance->onUnload();
                FreeLibrary(handle);
            }
        };

        std::vector<LoadedPlugin> mLoadedPlugins;

    public:
        PluginManager() = default;
        ~PluginManager() = default;

        PluginManager(const PluginManager &) = delete;
        PluginManager &operator=(const PluginManager &) = delete;

        bool loadAllPlugins();

        void pluginsOnLoaded();

        void unloadAllPlugins();

        void registerPlugin(IPlugin &plugin) {
            this->mLoadedPlugins.emplace_back(nullptr, &plugin);
        }

    private:
        void _onInitPlugins(const PluginInitContext &ctx);

        void _onUnInitPlugins();
    };

} // namespace sapphire::core