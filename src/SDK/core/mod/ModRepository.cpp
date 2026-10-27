/*
    管理 mod 的加载和卸载，解析 mod 的依赖等
*/

#include "ModRepository.h"
#include "SDK/api/sapphire/event/EventBus.h"
#include "SDK/api/sapphire/event/events/MinecraftGameInitFinishedEvent.h"
#include "SDK/api/sapphire/hook/Hook.h"
#include "SDK/api/sapphire/logger/Logger.h"
#include "SDK/api/sapphire/platform/Environment.h"
#include "SDK/api/src-client/common/client/game/ClientInstance.h"
#include "common/SemanticVersion.hpp"

namespace fs = std::filesystem;

namespace {

    using sapphire::SemanticVersion;

    std::regex validId{"^[a-z0-9_-]+:[a-z0-9_-]+$"};

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
        for (auto &dll : modInst->getManifest().mBinPath) {
            sapphire::debug("Loading mod {} at {}", modInst->getManifest().mId, dll.string());
            modInst->load();
            mLoadedMods.emplace_back(modInst);
        }
    }

    gModRepo = this;

    event::EventBus::getInstance().registerListener<event::MinecraftGameInitFinishedEvent>(
        [this](event::MinecraftGameInitFinishedEvent &e) {
            sapphire::ModInitContext ctx{
                e.mMinecraftGame,
                e.mClientInstance
            };
            this->publishOnInitEvent(ctx);
        }
    );

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
        if (mod->getIMod()) mod->getIMod()->onLoaded();
        mod->markLoaded();
    }

    return true;
}

void sapphire::core::ModRepository::unloadMods() {
    auto &&loadedMods = std::move(mLoadedMods);
    for (auto *mod : loadedMods | std::views::reverse) {
        if (mod->getIMod()) mod->getIMod()->onUnload();
    }
    mLoadedMods.clear();
    mLoadingOrder.clear();
    for (auto *mod : loadedMods | std::views::reverse) {
        mMods.erase(mod->getManifest().mId);
    }
    mMods.clear();

    OnDestroyMinecraftGameHook::unhook();
    gModRepo = this;
}

void sapphire::core::ModRepository::publishOnInitEvent(const sapphire::ModInitContext &ctx) {
    for (auto *mod : mLoadedMods) {
        if (mod->getIMod()) mod->getIMod()->onInit(ctx);
    }
}

void sapphire::core::ModRepository::publishOnUnInitEvent() {
    for (auto *mod : mLoadedMods | std::views::reverse) {
        if (mod->getIMod()) mod->getIMod()->onUnInit();
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
) try {
    auto manifestPath = modDir / "manifest.json";
    if (!fs::exists(manifestPath))
        return false;

    std::fstream manifestFs{manifestPath};

    if (!manifestFs.is_open())
        return false;
    nlohmann::json manifestJson{};
    manifestFs >> manifestJson;

    auto &manifestVersion = manifestJson["format_version"];
    if (!manifestVersion.is_number() || manifestVersion != 2)
        return false;

    auto &id = manifestJson["id"];
    if (!id.is_string()) return false;
    manifest.mId = id;
    if (!std::regex_match(manifest.mId, validId)) {
        return false;
    }

    auto &type = manifestJson["type"];
    if (!type.is_string()) return false;
    if (type == "mod")
        manifest.mType = ModManifest::ModType::SapphireMod;
    else if (type == "runtime")
        manifest.mType = ModManifest::ModType::Runtime;
    else
        return false;

    auto &version = manifestJson["version"];
    if (!version.is_string()) return false;

    manifest.mVersion = SemanticVersion::fromString(version).value_or({});

    auto &mainDll = manifestJson["bin"];
    if (!mainDll.is_array()) return false;
    for (auto &path : mainDll) {
        if (!path.is_string()) return false;
        manifest.mBinPath.emplace_back((modDir / path).lexically_normal());
    }

    auto &deps = manifestJson["dependencies"];
    if (deps.is_object() && !deps.empty()) {
        manifest.mDeps.reserve(deps.size());
        for (auto &&[k, v] : deps.items()) {
            if (!std::regex_match(k, validId)) {
                return false;
            }
            manifest.mDeps.emplace_back(k, v);
        }
    }
    return true;
} catch (std::exception &e) {
    sapphire::error("ModRepository: error loading manifest: {}", e.what());
    return false;
}

namespace {

    using sapphire::SemanticVersion;
    using sapphire::core::ModInstance;
    using sapphire::core::ModManifest;

    // 两个特殊依赖：游戏本体与框架自身。
    // 它们不是 mMods 里的 mod，而是直接由传入的版本号满足。
    constexpr std::string_view kGameDependencyId = "minecraft:game";
    constexpr std::string_view kCoreDependencyId = "sapphire:core";

    // 一条依赖需求：mRequiredBy 要求 mId 满足 mVersionConstraint
    struct DependencyRequest {
        std::string          mId;
        std::string          mVersionConstraint;
        std::string          mRequiredBy; // 发起需求的 mod id；为空表示“已安装的 mod 列表”本身
        ModManifest::ModType mRequiredByType = ModManifest::ModType::SapphireMod;
    };

    std::string shorten(std::string text, std::size_t maxLength = 1024) {
        if (text.size() <= maxLength) return text;
        text.resize(maxLength);
        text += "...";
        return text;
    }

    std::string joinIds(const std::vector<std::string> &ids) {
        std::string result;
        for (auto &&id : ids) {
            if (!result.empty()) result += ", ";
            result += id;
        }
        return result;
    }

    std::string describeRequester(const DependencyRequest &request) {
        if (request.mRequiredBy.empty()) return "the installed mod list";
        return std::format("mod '{}'", request.mRequiredBy);
    }

    std::string formatVersions(const std::vector<ModInstance *> &candidates) {
        std::string result;
        for (auto *candidate : candidates) {
            if (!result.empty()) result += ", ";
            result += candidate->getManifest().mVersion.toString();
        }
        return result.empty() ? "<none>" : result;
    }

    class DependencyResolver {
    public:
        struct Solution {
            std::map<std::string, ModInstance *>                                    mSelected;
            std::map<std::string, std::vector<DependencyRequest>>                   mConstraints;
            std::map<std::string, std::vector<std::pair<std::size_t, std::string>>> mRejectedCandidates;
        };

        DependencyResolver(
            std::map<std::string, std::vector<ModInstance>> &mods,
            const SemanticVersion                           &gameVersion,
            const SemanticVersion                           &sapphireVersion
        ) :
            mMods(mods),
            mGameVersion(gameVersion),
            mSapphireVersion(sapphireVersion) {
            for (auto &&[id, instances] : mMods) {
                auto &candidates = mCandidates[id];
                candidates.reserve(instances.size());
                for (auto &&instance : instances) {
                    candidates.push_back(&instance);
                }
                std::ranges::stable_sort(candidates, [](const ModInstance *lhs, const ModInstance *rhs) {
                    return lhs->getManifest().mVersion > rhs->getManifest().mVersion;
                });
            }
        }

        bool solve(const std::vector<std::string> &roots, Solution &solution, std::string &failure);

        [[nodiscard]] const std::vector<ModInstance *> *candidatesOf(std::string_view id) const {
            auto it = mCandidates.find(std::string{id});
            return it == mCandidates.end() ? nullptr : &it->second;
        }

        // mod id 的类型以它最新的版本为准（正常情况下一个 id 的所有版本类型一致）
        [[nodiscard]] ModManifest::ModType typeOf(std::string_view id) const {
            const auto *candidates = candidatesOf(id);
            if (!candidates || candidates->empty()) return ModManifest::ModType::SapphireMod;
            return candidates->front()->getManifest().mType;
        }

        // 该 id 是否被某个 mod 作为依赖请求过（无论请求方最终是否加载成功）
        [[nodiscard]] bool wasRequiredAsDependency(std::string_view id) const {
            return mRequiredIds.contains(std::string{id});
        }

    private:
        struct State {
            std::map<std::string, std::size_t>                    mChosen;      // id -> 候选版本下标
            std::map<std::string, std::vector<DependencyRequest>> mConstraints; // id -> 已生效的需求
            std::vector<std::string>                              mPath;        // 当前依赖链，用于环检测
        };

        bool _resolve(const std::vector<DependencyRequest> &pending, State &state, std::string &failure);

        [[nodiscard]] ModInstance *_candidate(std::string_view id, std::size_t index) const;

        // 版本区间只解析一次并缓存，避免回溯时重复解析与重复报错
        const VersionConstraint &_constraint(std::string_view constraintStr);

        std::map<std::string, std::vector<ModInstance>> &mMods;
        const SemanticVersion                           &mGameVersion;
        const SemanticVersion                           &mSapphireVersion;

        std::map<std::string, std::vector<ModInstance *>>                       mCandidates;
        std::map<std::string, VersionConstraint>                                mConstraintCache;
        std::set<std::string>                                                   mRequiredIds;
        std::map<std::string, std::vector<std::pair<std::size_t, std::string>>> mRejectedCandidates;

        std::size_t mSteps = 0;

        static constexpr std::size_t kMaxSteps = 200000;
    };

    bool DependencyResolver::solve(
        const std::vector<std::string> &roots, Solution &solution, std::string &failure
    ) {
        mSteps = 0;
        mRejectedCandidates.clear();
        solution = Solution{};

        State                          state;
        std::vector<DependencyRequest> pending;
        pending.reserve(roots.size());
        for (auto &&root : roots) {
            pending.push_back(DependencyRequest{root, "*", {}, ModManifest::ModType::SapphireMod});
        }

        if (!_resolve(pending, state, failure)) return false;

        for (auto &&[id, index] : state.mChosen) {
            solution.mSelected.emplace(id, _candidate(id, index));
        }
        solution.mConstraints = std::move(state.mConstraints);
        solution.mRejectedCandidates = std::move(mRejectedCandidates);
        return true;
    }

    bool DependencyResolver::_resolve(
        const std::vector<DependencyRequest> &pending, State &state, std::string &failure
    ) {
        if (pending.empty()) return true;

        if (++mSteps > kMaxSteps) {
            failure = "dependency resolution gave up after too many steps, the dependency graph is too complex";
            return false;
        }

        const DependencyRequest        request = pending.front();
        std::vector<DependencyRequest> rest(pending.begin() + 1, pending.end());

        const VersionConstraint &constraint = _constraint(request.mVersionConstraint);
        if (!constraint.isValid()) {
            failure = std::format(
                "{} requires '{}' with an invalid version constraint '{}'",
                describeRequester(request),
                request.mId,
                request.mVersionConstraint
            );
            return false;
        }

        const std::string_view requestId{request.mId};
        if (requestId == kGameDependencyId || requestId == kCoreDependencyId) {
            const bool             isGame = requestId == kGameDependencyId;
            const SemanticVersion &provided = isGame ? mGameVersion : mSapphireVersion;
            if (constraint.isSatisfiedBy(provided)) {
                return _resolve(rest, state, failure);
            }
            failure = std::format(
                "{} requires {} '{}', but the current {} version is {}",
                describeRequester(request),
                request.mId,
                request.mVersionConstraint,
                isGame ? "game" : "sapphire",
                provided.toString()
            );
            return false;
        }

        if (std::ranges::find(state.mPath, request.mId) != state.mPath.end()) {
            std::string chain;
            for (auto &&id : state.mPath) {
                chain += id;
                chain += " -> ";
            }
            chain += request.mId;
            failure = std::format("circular dependency detected: {}", chain);
            return false;
        }

        if (auto chosenIt = state.mChosen.find(request.mId); chosenIt != state.mChosen.end()) {
            const ModInstance *chosen = _candidate(request.mId, chosenIt->second);
            const ModManifest &manifest = chosen->getManifest();

            if (!constraint.isSatisfiedBy(manifest.mVersion)) {
                failure = std::format(
                    "{} requires {} '{}', but version {} has already been selected for {}",
                    describeRequester(request),
                    request.mId,
                    request.mVersionConstraint,
                    manifest.mVersion.toString(),
                    request.mId
                );
                return false;
            }

            if (manifest.mType == ModManifest::ModType::SapphireMod
                && request.mRequiredByType == ModManifest::ModType::Runtime) {
                failure = std::format(
                    "runtime mod '{}' is not allowed to depend on sapphire mod '{}'",
                    request.mRequiredBy,
                    request.mId
                );
                return false;
            }

            state.mConstraints[request.mId].push_back(request);
            if (_resolve(rest, state, failure)) return true;
            state.mConstraints[request.mId].pop_back();
            return false;
        }

        const auto *candidates = candidatesOf(request.mId);
        if (!candidates || candidates->empty()) {
            failure = std::format(
                "{} requires '{}', but no such mod is installed",
                describeRequester(request),
                request.mId
            );
            return false;
        }

        if (!request.mRequiredBy.empty()) mRequiredIds.insert(request.mId);

        bool                                             anyVersionMatched = false;
        bool                                             remainingFailed = false;
        std::string                                      remainingFailureOfOthers;
        std::vector<std::pair<std::size_t, std::string>> failures;

        for (std::size_t i = 0; i < candidates->size(); ++i) {
            const ModInstance *candidate = (*candidates)[i];
            const ModManifest &manifest = candidate->getManifest();

            if (!constraint.isSatisfiedBy(manifest.mVersion)) continue;
            anyVersionMatched = true;

            if (manifest.mType == ModManifest::ModType::SapphireMod
                && request.mRequiredByType == ModManifest::ModType::Runtime) {
                failures.emplace_back(
                    i,
                    std::format(
                        "runtime mod '{}' is not allowed to depend on sapphire mod '{}'",
                        request.mRequiredBy,
                        request.mId
                    )
                );
                continue;
            }

            const State snapshot = state;

            state.mChosen[request.mId] = i;
            state.mConstraints[request.mId].push_back(request);

            std::vector<DependencyRequest> deps;
            deps.reserve(manifest.mDeps.size());
            for (auto &&dep : manifest.mDeps) {
                deps.push_back(DependencyRequest{dep.mId, dep.mVersionConstraint, manifest.mId, manifest.mType});
            }

            state.mPath.push_back(request.mId);
            std::string dependencyFailure;
            const bool  dependenciesResolved = _resolve(deps, state, dependencyFailure);
            state.mPath.pop_back();

            bool        remainingResolved = false;
            std::string remainingFailure;
            if (dependenciesResolved) {
                remainingResolved = _resolve(rest, state, remainingFailure);
            }

            if (dependenciesResolved && remainingResolved) {
                for (auto &&[rejectedIndex, reason] : failures) {
                    mRejectedCandidates[request.mId].emplace_back(rejectedIndex, reason);
                }
                return true;
            }

            state = snapshot;

            if (dependenciesResolved) {
                remainingFailed = true;
                remainingFailureOfOthers = std::move(remainingFailure);
            } else {
                failures.emplace_back(i, std::move(dependencyFailure));
            }
        }

        if (!anyVersionMatched) {
            failure = std::format(
                "{} requires {} '{}', but none of the installed versions ({}) satisfies it",
                describeRequester(request),
                request.mId,
                request.mVersionConstraint,
                formatVersions(*candidates)
            );
            return false;
        }

        if (failures.empty() && remainingFailed) {
            failure = std::move(remainingFailureOfOthers);
            return false;
        }

        std::string summary;
        for (std::size_t i = 0; i < failures.size() && i < 3; ++i) {
            if (!summary.empty()) summary += "; ";
            summary += std::format(
                "version {} cannot be used because {}",
                (*candidates)[failures[i].first]->getManifest().mVersion.toString(),
                failures[i].second
            );
        }
        if (failures.size() > 3) {
            summary += std::format("; ({} more version(s) omitted)", failures.size() - 3);
        }
        if (remainingFailed) {
            if (!summary.empty()) summary += "; ";
            summary += std::format("other requirements cannot be satisfied: {}", remainingFailureOfOthers);
        }

        failure = std::format(
            "{} requires {}, but no installed version can be loaded ({})",
            describeRequester(request),
            request.mId,
            summary
        );
        return false;
    }

    ModInstance *DependencyResolver::_candidate(std::string_view id, std::size_t index) const {
        const auto *candidates = candidatesOf(id);
        assert(candidates && index < candidates->size());
        return (*candidates)[index];
    }

    const VersionConstraint &DependencyResolver::_constraint(std::string_view constraintStr) {
        std::string key{constraintStr};
        auto        it = mConstraintCache.find(key);
        if (it == mConstraintCache.end()) {
            it = mConstraintCache.emplace(key, VersionConstraint{key}).first;
        }
        return it->second;
    }

    bool topologicalOrder(
        const std::vector<std::string>             &ids,
        const std::map<std::string, ModInstance *> &selected,
        std::vector<std::string>                   &order,
        std::vector<std::string>                   &cyclic
    ) {
        const std::set<std::string> idSet(ids.begin(), ids.end());

        std::map<std::string, std::size_t>              inDegree;
        std::map<std::string, std::vector<std::string>> dependents;

        for (auto &&id : ids) inDegree[id] = 0;
        for (auto &&id : ids) {
            for (auto &&dep : selected.at(id)->getManifest().mDeps) {
                if (!idSet.contains(dep.mId)) continue;
                ++inDegree[id];
                dependents[dep.mId].push_back(id);
            }
        }

        std::set<std::string> ready;
        for (auto &&[id, degree] : inDegree) {
            if (degree == 0) ready.insert(id);
        }

        order.clear();
        order.reserve(ids.size());
        while (!ready.empty()) {
            const std::string id = *ready.begin();
            ready.erase(ready.begin());
            order.push_back(id);
            for (auto &&dependent : dependents[id]) {
                if (--inDegree[dependent] == 0) ready.insert(dependent);
            }
        }

        cyclic.clear();
        for (auto &&id : ids) {
            if (std::ranges::find(order, id) == order.end()) cyclic.push_back(id);
        }
        return cyclic.empty();
    }

} // namespace

void sapphire::core::ModRepository::_resolveModDependencies(
    const SemanticVersion &MCVersion, const SemanticVersion &sapphireVersion
) {
    mLoadingOrder.clear();

    if (mMods.empty()) {
        sapphire::info("ModRepository: no mod installed");
        return;
    }

    DependencyResolver resolver{mMods, MCVersion, sapphireVersion};

    std::vector<std::string> sapphireModIds;
    std::vector<std::string> runtimeIds;
    for (auto &&[id, instances] : mMods) {
        if (instances.empty()) continue;
        if (resolver.typeOf(id) == ModManifest::ModType::Runtime) {
            runtimeIds.push_back(id);
        } else {
            sapphireModIds.push_back(id);
        }
    }

    std::vector<std::string>     committed;
    DependencyResolver::Solution solution;

    for (auto &&root : sapphireModIds) {
        std::vector<std::string>     trial = committed;
        DependencyResolver::Solution trialSolution;
        std::string                  failure;

        trial.push_back(root);

        if (resolver.solve(trial, trialSolution, failure)) {
            committed = std::move(trial);
            solution = std::move(trialSolution);
            continue;
        }

        sapphire::error(
            "ModRepository: cannot resolve dependencies for mod '{}', none of its {} installed version(s) can be loaded, it will be skipped. Reason: {}",
            root,
            mMods.at(root).size(),
            shorten(std::move(failure))
        );
    }

    std::vector<std::string> selectedRuntimes;
    std::vector<std::string> selectedSapphireMods;
    for (auto &&[id, instance] : solution.mSelected) {
        if (instance->getManifest().mType == ModManifest::ModType::Runtime) {
            selectedRuntimes.push_back(id);
        } else {
            selectedSapphireMods.push_back(id);
        }
    }

    std::vector<std::string> runtimeOrder;
    std::vector<std::string> sapphireModOrder;
    std::vector<std::string> cyclic;

    if (!topologicalOrder(selectedRuntimes, solution.mSelected, runtimeOrder, cyclic)) {
        sapphire::error(
            "ModRepository: circular dependency detected between runtime mods, they will be skipped: {}",
            joinIds(cyclic)
        );
    }
    if (!topologicalOrder(selectedSapphireMods, solution.mSelected, sapphireModOrder, cyclic)) {
        sapphire::error(
            "ModRepository: circular dependency detected between mods, they will be skipped: {}",
            joinIds(cyclic)
        );
    }

    mLoadingOrder.reserve(runtimeOrder.size() + sapphireModOrder.size());
    for (auto &&id : runtimeOrder) {
        mLoadingOrder.push_back(solution.mSelected.at(id));
    }
    for (auto &&id : sapphireModOrder) {
        mLoadingOrder.push_back(solution.mSelected.at(id));
    }

    for (auto &&id : runtimeIds) {
        if (solution.mSelected.contains(id)) continue;
        if (resolver.wasRequiredAsDependency(id)) {
            sapphire::warn(
                "ModRepository: runtime mod '{}' is required by other mods but could not be loaded, the mods depending on it are skipped as well",
                id
            );
        } else {
            sapphire::warn(
                "ModRepository: runtime mod '{}' is not depended on by any mod, it will not be loaded",
                id
            );
        }
    }

    for (auto &&[id, instance] : solution.mSelected) {
        const auto *candidates = resolver.candidatesOf(id);
        if (!candidates || candidates->empty()) continue;

        const ModInstance *newest = candidates->front();
        if (!(instance->getManifest().mVersion < newest->getManifest().mVersion)) continue;

        std::string reasons;

        if (auto it = solution.mConstraints.find(id); it != solution.mConstraints.end()) {
            for (auto &&constraint : it->second) {
                if (constraint.mRequiredBy.empty()) continue;
                if (!reasons.empty()) reasons += "; ";
                reasons += std::format(
                    "required by '{}' as '{}'",
                    constraint.mRequiredBy,
                    constraint.mVersionConstraint
                );
            }
        }

        if (auto it = solution.mRejectedCandidates.find(id); it != solution.mRejectedCandidates.end()) {
            for (auto &&[index, reason] : it->second) {
                if (index >= candidates->size()) continue;
                if (!reasons.empty()) reasons += "; ";
                reasons += std::format(
                    "version {} rejected: {}",
                    (*candidates)[index]->getManifest().mVersion.toString(),
                    reason
                );
            }
        }

        if (reasons.empty()) {
            reasons = "the newest version conflicts with the resolved dependency graph";
        }

        sapphire::warn(
            "ModRepository: mod '{}' will be loaded at version {} instead of the newest {}, because: {}",
            id,
            instance->getManifest().mVersion.toString(),
            newest->getManifest().mVersion.toString(),
            shorten(std::move(reasons))
        );
    }

    if (!mLoadingOrder.empty()) {
        std::string order;
        for (auto *instance : mLoadingOrder) {
            if (!order.empty()) order += " -> ";
            order += std::format(
                "{}@{}",
                instance->getManifest().mId,
                instance->getManifest().mVersion.toString()
            );
        }
        sapphire::debug("ModRepository: mod loading order: {}", order);
    }
}
