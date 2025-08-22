#pragma once

#include <minwindef.h>
#include <vector>
#include <ranges>
#include "IPlugin.h"

namespace sapphire {

    class PluginManager {
        friend class Core;
        friend class IatPatcher;

        class LoadedPlugin {
        public:
            HMODULE  handle;
            IPlugin *instance;
            bool     inited = false;

            LoadedPlugin(HMODULE handle, IPlugin *instance) :
                handle(handle), instance(instance) {}

            ~LoadedPlugin() {
                if (inited) instance->uninit();
                FreeLibrary(handle);
            }
        };

        std::vector<LoadedPlugin> mLoadedPlugins;

        PluginManager() = default;
        ~PluginManager() {
            this->uninitAllPlugins();
        }

        void initAllPlugins() {
            for (auto &&plg : this->mLoadedPlugins) {
                plg.instance->init();
                plg.inited = true;
            }
        }

        void uninitAllPlugins() {
            for (auto &&plg : std::views::reverse(this->mLoadedPlugins)) {
                if (plg.inited)
                    plg.instance->uninit();
                plg.inited = false;
            }
        }

        void freeAllPlugins() {
            for (auto &&plg : std::views::reverse(this->mLoadedPlugins)) {
                if (plg.handle)
                    FreeLibrary(plg.handle);
                plg.handle = nullptr;
            }
            this->mLoadedPlugins.clear();
        }

    public:
        PluginManager(const PluginManager &) = delete;
        PluginManager &operator=(const PluginManager &) = delete;

        void registerPlugin(IPlugin &plugin) {
            this->mLoadedPlugins.emplace_back(nullptr, &plugin);
        }
    };

} // namespace sapphire