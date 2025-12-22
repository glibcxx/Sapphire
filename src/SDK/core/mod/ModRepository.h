#pragma once

#include "ModInstance.h"
#include "ModManifest.h"
#include <vector>

namespace sapphire::core {

    class ModRepository {
        std::map<std::string, std::vector<ModInstance>> mMods;

        std::vector<ModInstance *> mLoadingOrder;
        std::vector<ModInstance *> mLoadedMods;

    public:
        bool loadMods();

        void unloadMods();

        void publishOnInitEvent(const ModInitContext &ctx);

        void publishOnUnInitEvent();

    private:
        void _loadManifests(const std::filesystem::path &modsDir);

        bool _loadManifest(const std::filesystem::path &modDir, ModManifest &manifest);

        void _resolveModDependencies(const SemanticVersion &MCVersion, const SemanticVersion &sapphireVersion);
    };

} // namespace sapphire::core