#pragma once

#include <minwindef.h>
#include <vector>
#include <ranges>
#include "IPlugin.h"

namespace sapphire {

    class PluginManager {
        friend class Core;
        friend class IatPatcher;

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

        PluginManager() = default;
        ~PluginManager() = default;

        void pluginsOnLoaded();

        void unloadAllPlugins();

        void _onInitPlugins();

        void _onUnInitPlugins();

    public:
        PluginManager(const PluginManager &) = delete;
        PluginManager &operator=(const PluginManager &) = delete;

        void registerPlugin(IPlugin &plugin) {
            this->mLoadedPlugins.emplace_back(nullptr, &plugin);
        }
    };

} // namespace sapphire