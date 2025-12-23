#include "ModRepository.h"
#include "SDK/api/sapphire/hook/Hook.h"
#include "SDK/api/sapphire/logger/Logger.h"
#include "SDK/api/sapphire/platform/Environment.h"
#include "SDK/api/src-client/common/client/game/ClientInstance.h"
#include "SDK/api/src-client/common/client/game/MinecraftGame.h"
#include "common/SemanticVersion.hpp"
#include <ranges>

namespace fs = std::filesystem;

namespace {

    using sapphire::SemanticVersion;

    void ltrim(std::string &s) {
        s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char ch) {
                    return !std::isspace(ch);
                }));
    }

    void rtrim(std::string &s) {
        s.erase(std::find_if(s.rbegin(), s.rend(), [](unsigned char ch) {
                    return !std::isspace(ch);
                }).base(),
                s.end());
    }

    void trim(std::string &s) {
        ltrim(s);
        rtrim(s);
    }

    std::vector<std::string> split(const std::string &s, char delimiter) {
        std::vector<std::string> tokens;
        std::string              token;
        std::istringstream       tokenStream(s);
        while (std::getline(tokenStream, token, delimiter)) {
            tokens.push_back(token);
        }
        return tokens;
    }

    class VersionConstraint {
    public:
        explicit VersionConstraint(std::string_view constraintStr) {
            std::string constraint(constraintStr);
            trim(constraint);

            if (constraint.empty() || constraint == "*") {
                return; // Empty checks means always satisfied
            }

            std::stringstream ss(constraint);
            std::string       segment;
            while (ss >> segment) {
                if (!parseSegment(segment)) {
                    mIsValid = false;
                    sapphire::error("Invalid version constraint: {}", constraint);
                    mChecks.clear();
                    break;
                }
            }
        }

        bool isSatisfiedBy(const SemanticVersion &version) const {
            if (!mIsValid) return false;
            if (mChecks.empty()) return true;
            return std::all_of(mChecks.begin(), mChecks.end(), [&](const auto &check) { return check(version); });
        }

        bool isValid() const { return mIsValid; }

    private:
        bool parseSegment(std::string_view segment) {
            if (segment.find_first_of("xX*") != std::string_view::npos) {
                return parseWildcardSegment(segment);
            }
            return parseOperatorSegment(segment);
        }

        bool parseWildcardSegment(std::string_view segment) {
            std::string_view baseVersionStr = segment.substr(0, segment.find_first_of("xX*"));
            if (!baseVersionStr.empty() && baseVersionStr.back() == '.') {
                baseVersionStr.remove_suffix(1);
            }

            auto parts = split(std::string(baseVersionStr), '.');
            int  major = -1, minor = -1;

            try {
                if (parts.size() >= 1 && !parts[0].empty()) {
                    major = std::stoi(parts[0]);
                }
                if (parts.size() >= 2 && !parts[1].empty()) {
                    minor = std::stoi(parts[1]);
                }
            } catch (const std::exception &e) {
                sapphire::error("Invalid wildcard version format: {}: {}", std::string(segment), e.what());
                return false;
            }

            mChecks.emplace_back([major, minor](const SemanticVersion &v) {
                if (major != -1 && v.mMajor != major) return false;
                if (minor != -1 && v.mMinor != minor) return false;
                return true;
            });
            return true;
        }

        bool parseOperatorSegment(std::string_view segment) {
            std::string_view op;
            std::string_view versionSv;

            if (segment.starts_with(">=")) {
                op = ">=";
            } else if (segment.starts_with("<=")) {
                op = "<=";
            } else if (segment.starts_with(">")) {
                op = ">";
            } else if (segment.starts_with("<")) {
                op = "<";
            } else if (segment.starts_with("=")) {
                op = "=";
            }

            if (!op.empty()) {
                versionSv = segment.substr(op.length());
            } else {
                op = "="; // Default operator
                versionSv = segment;
            }

            auto versionOpt = SemanticVersion::fromString(versionSv);
            if (!versionOpt) {
                sapphire::error("Invalid version string in constraint: {}", std::string(versionSv));
                return false;
            }
            SemanticVersion targetVersion = *versionOpt;

            if (op == ">=") {
                mChecks.emplace_back(
                    [targetVersion](const SemanticVersion &v) { return v >= targetVersion; }
                );
            } else if (op == "<=") {
                mChecks.emplace_back(
                    [targetVersion](const SemanticVersion &v) { return v <= targetVersion; }
                );
            } else if (op == ">") {
                mChecks.emplace_back(
                    [targetVersion](const SemanticVersion &v) { return v > targetVersion; }
                );
            } else if (op == "<") {
                mChecks.emplace_back(
                    [targetVersion](const SemanticVersion &v) { return v < targetVersion; }
                );
            } else if (op == "=") {
                mChecks.emplace_back([targetVersion](const SemanticVersion &v) {
                    return v.mMajor == targetVersion.mMajor
                        && v.mMinor == targetVersion.mMinor
                        && v.mPatch == targetVersion.mPatch;
                });
            } else {
                return false; // unreachable
            }

            return true;
        }

        std::vector<std::function<bool(const SemanticVersion &)>> mChecks;
        bool                                                      mIsValid = true;
    };

    sapphire::core::ModRepository *gModRepo = nullptr;

    HOOK_TYPE(
        OnMinecraftGameInitCompleteHook,
        MinecraftGame,
        sapphire::HookPriority::Normal,
        MinecraftGame::_initFinish,
        SerialWorkList::WorkResult,
        std::shared_ptr<MinecraftGame::InitContext> &initContext
    ) {
        auto res = this->origin(initContext);
        if (res == SerialWorkList::WorkResult::Complete) {
            sapphire::ModInitContext ctx{
                *this,
                *initContext->mIncompletePrimaryClient
            };
            gModRepo->publishOnInitEvent(ctx);
        }
        return res;
    }

    HOOK_TYPE(
        OnDestroyMinecraftGameHook,
        ClientInstance,
        sapphire::HookPriority::Normal,
        ClientInstance::onDestroyMinecraftGame,
        void
    ) {
        gModRepo->publishOnUnInitEvent();
        this->origin();
    }

} // namespace

bool sapphire::core::ModRepository::loadMods() {
    auto    &env = platform::Environment::getInstance();
    fs::path modsDir = env.getSapphireHomePath() / "mods";
    if (!fs::exists(modsDir)) {
        fs::create_directory(modsDir);
    }
    if (!fs::is_directory(modsDir)) {
        sapphire::error("ModRepository: mods not found: {}", modsDir.string());
        return false;
    }

    _loadManifests(modsDir);

    _resolveModDependencies(env.getGameVersion(), env.getSapphireVersion());

    for (auto *modInst : mLoadingOrder) {
        sapphire::debug("Loading mod {} at {}", modInst->getManifest().mId, modInst->getManifest().mMainDllPath.string());
        if (modInst->load()) {
            mLoadedMods.emplace_back(modInst);
        }
    }

    gModRepo = this;
    if (!OnMinecraftGameInitCompleteHook::hook()) {
        sapphire::error("OnMinecraftGameInitCompleteHook hook failed!");
        return false;
    }

    if (!OnDestroyMinecraftGameHook::hook()) {
        sapphire::error("OnDestroyMinecraftGameHook hook failed!");
        return false;
    }

    for (auto &&[id, mods] : mMods) {
        for (auto &&mod : mods) {
            mod.markIgnored();
        }
    }

    for (auto *mod : mLoadedMods) {
        mod->getIMod().onLoaded();
        mod->markLoaded();
    }

    return true;
}

void sapphire::core::ModRepository::unloadMods() {
    auto &&loadedMods = std::move(mLoadedMods);
    for (auto *mod : loadedMods | std::views::reverse) {
        mod->getIMod().onUnload();
    }
    mLoadedMods.clear();
    mLoadingOrder.clear();
    for (auto *mod : loadedMods | std::views::reverse) {
        mMods.erase(mod->getManifest().mId);
    }
    mMods.clear();

    OnDestroyMinecraftGameHook::unhook();
    OnMinecraftGameInitCompleteHook::unhook();
    gModRepo = this;
}

void sapphire::core::ModRepository::publishOnInitEvent(const sapphire::ModInitContext &ctx) {
    for (auto *mod : mLoadedMods) {
        mod->getIMod().onInit(ctx);
    }
}

void sapphire::core::ModRepository::publishOnUnInitEvent() {
    for (auto *mod : mLoadedMods | std::views::reverse) {
        mod->getIMod().onUnInit();
    }
}

void sapphire::core::ModRepository::_loadManifests(const std::filesystem::path &modsDir) {
    for (auto &&entry : fs::directory_iterator{modsDir}) {
        if (!entry.is_directory()) continue;

        ModManifest manifest;
        if (!_loadManifest(entry.path(), manifest)) {
            sapphire::error("ModRepository: Unable to load manifest: {}", entry.path().string());
            continue;
        }
        mMods[manifest.mId].emplace_back(std::move(manifest));
    }
    for (auto &&[modId, mods] : mMods) {
        std::ranges::sort(mods, [](const ModInstance &lhs, const ModInstance &rhs) {
            auto &_lhs = lhs.getManifest();
            auto &_rhs = rhs.getManifest();
            return _lhs.mVersion > _rhs.mVersion;
        });
    }
}

bool sapphire::core::ModRepository::_loadManifest(
    const std::filesystem::path &modDir, sapphire::core::ModManifest &manifest
) {
    auto manifestPath = modDir / "manifest.json";
    if (!fs::exists(manifestPath))
        return false;

    std::fstream manifestFs{manifestPath};

    if (!manifestFs.is_open())
        return false;
    nlohmann::json manifestJson{};
    manifestFs >> manifestJson;

    auto &id = manifestJson["id"];
    if (!id.is_string()) return false;
    manifest.mId = id;

    auto &version = manifestJson["version"];
    if (!version.is_string()) return false;

    manifest.mVersion = SemanticVersion::fromString(version).value_or({});

    auto &mainDll = manifestJson["main_dll"];
    if (!mainDll.is_string()) return false;

    manifest.mMainDllPath = (modDir / mainDll).lexically_normal();

    auto &deps = manifestJson["dependencies"];
    if (!deps.is_object() || deps.empty()) return false;

    if (!deps.contains("minecraft")) return false;

    manifest.mDeps.reserve(deps.size());
    for (auto &&[k, v] : deps.items()) {
        manifest.mDeps.emplace_back(k, v);
    }
    return true;
}

void sapphire::core::ModRepository::_resolveModDependencies(
    const SemanticVersion &MCVersion, const SemanticVersion &sapphireVersion
) {
    std::unordered_map<std::string, std::vector<ModInstance *>> compatibleMods;
    for (auto &[modId, mods] : mMods) {
        bool anyVersionCompatible = false;
        for (auto &mod : mods) {
            bool compatible = false;
            for (const auto &dep : mod.getManifest().mDeps) {
                if (dep.mId == "minecraft") {
                    VersionConstraint constraint(dep.mVersionConstraint);
                    if (constraint.isValid() && constraint.isSatisfiedBy(MCVersion)) {
                        compatible = true;
                        break;
                    }
                } else if (dep.mId == "Sapphire:core") {
                    VersionConstraint constraint(dep.mVersionConstraint);
                    if (!constraint.isValid() || !constraint.isSatisfiedBy(sapphireVersion)) {
                        break;
                    }
                }
            }
            if (compatible) {
                anyVersionCompatible = true;
                compatibleMods[modId].push_back(&mod);
            }
        }
        if (!anyVersionCompatible) {
            sapphire::error(
                "dependency 'minecraft' or 'Sapphire:core' of any version of Mod '{}' is not satisfied.",
                modId
            );
        }
    }

    std::unordered_map<std::string, const ModInstance *> resolvedMods;
    std::unordered_set<std::string>                      visiting;

    auto resolve = [&](auto &self, const std::string &modId) -> bool {
        visiting.insert(modId);

        auto it = compatibleMods.find(modId);
        if (it == compatibleMods.end()) {
            sapphire::error("Dependency '{}' not found.", modId);
            visiting.erase(modId);
            return false;
        }

        for (const auto *modInstance : it->second) {
            bool dependenciesMet = true;
            for (const auto &dep : modInstance->getManifest().mDeps) {
                if (dep.mId == "minecraft" || dep.mId == "Sapphire:core") continue;

                if (visiting.count(dep.mId)) {
                    sapphire::error("Circular dependency detected: {} -> {}", modId, dep.mId);
                    dependenciesMet = false;
                    break;
                }

                auto resolvedIt = resolvedMods.find(dep.mId);
                if (resolvedIt != resolvedMods.end()) {
                    VersionConstraint constraint(dep.mVersionConstraint);
                    if (!constraint.isValid()
                        || !constraint.isSatisfiedBy(resolvedIt->second->getManifest().mVersion)) {
                        dependenciesMet = false;
                        break;
                    }
                } else {
                    if (!self(self, dep.mId)) {
                        dependenciesMet = false;
                        break;
                    }
                    resolvedIt = resolvedMods.find(dep.mId);
                    VersionConstraint constraint(dep.mVersionConstraint);
                    if (resolvedIt == resolvedMods.end()
                        || !constraint.isValid()
                        || !constraint.isSatisfiedBy(resolvedIt->second->getManifest().mVersion)) {
                        dependenciesMet = false;
                        break;
                    }
                }
            }

            if (dependenciesMet) {
                resolvedMods[modId] = modInstance;
                visiting.erase(modId);
                return true;
            }
        }

        sapphire::error("Failed to resolve dependencies for mod: {}. No compatible version found.", modId);
        visiting.erase(modId);
        return false;
    };

    for (const auto &[modId, _] : compatibleMods) {
        if (resolvedMods.find(modId) == resolvedMods.end()) {
            resolve(resolve, modId);
        }
    }

    // Topological Sort
    std::unordered_map<std::string, std::vector<std::string>> graph;
    std::unordered_map<std::string, int>                      inDegree;

    for (const auto &[modId, _] : resolvedMods) {
        inDegree[modId] = 0;
    }

    for (const auto &[modId, modInstance] : resolvedMods) {
        for (const auto &dep : modInstance->getManifest().mDeps) {
            if (dep.mId != "minecraft" && resolvedMods.count(dep.mId)) {
                graph[dep.mId].push_back(modId);
                inDegree[modId]++;
            }
        }
    }

    std::stack<std::string> stack;
    for (const auto &[modId, degree] : inDegree) {
        if (degree == 0) {
            stack.push(modId);
        }
    }

    while (!stack.empty()) {
        std::string u = stack.top();
        stack.pop();
        mLoadingOrder.push_back(const_cast<ModInstance *>(resolvedMods[u]));

        if (graph.count(u)) {
            for (const auto &v : graph[u]) {
                if (--inDegree[v] == 0) {
                    stack.push(v);
                }
            }
        }
    }

    if (mLoadingOrder.size() != resolvedMods.size()) {
        sapphire::error("Could not create a valid loading order for all mods. "
                        "There might be a complex circular dependency.");
        mLoadingOrder.clear();
    }
}
