#pragma once

#include <string>
#include <filesystem>
#include <fstream>
#include <mutex>
#include <optional>
#include <nlohmann/json.hpp>

#include "SDK/core/Core.h"
#include "SDK/api/sapphire/logger/Logger.h"
#include "imgui.h"

namespace sapphire::inline config {

    class Config : public std::enable_shared_from_this<Config> {
        nlohmann::json        mData;
        std::filesystem::path mPath;
        bool                  mIsDirty = false;

    public:
        Config(const std::filesystem::path &path) :
            mPath(path) {
        }

        Config(const Config &) = delete;

        template <typename T>
        void set(const std::string &key, T &&val) {
            mData[key] = val;
            mIsDirty = true;
        }

        template <typename T>
        decltype(auto) get(const std::string &key) {
            return mData[key].get<T>();
        }

        void save() {
            std::ofstream ofs{mPath};
            if (ofs.is_open()) {
                ofs << mData.dump(4);
            }
            Logger::Debug("[Config] saved: {}", mPath.string());
            mIsDirty = false;
        }

        void load() {
            std::ifstream ifs{mPath};
            if (ifs.is_open()) {
                ifs >> mData;
                Logger::Debug("[Config] loaded: {}", mPath.string());
            } else {
                Logger::Debug("[Config] new Config: {}", mPath.string());
                save();
            }
            mIsDirty = false;
        }

        bool isDirty() const {
            return mIsDirty;
        }
    };

    class ConfigManager {
        std::unordered_map<std::string, std::shared_ptr<Config>> mConfigs;

        std::filesystem::path mConfigDir;

        ConfigManager();

    public:
        SDK_API static ConfigManager &getInstance();

        void save() {
            for (auto &&[filename, config] : mConfigs) {
                if (config->isDirty())
                    config->save();
            }
        }

        Config &operator[](const std::string &filename) {
            auto it = mConfigs.find(filename);
            if (it == mConfigs.end()) {
                it = mConfigs.emplace(filename, std::make_shared<Config>(mConfigDir / filename)).first;
                it->second->load();
            }
            return *it->second;
        }
    };

} // namespace sapphire::inline config