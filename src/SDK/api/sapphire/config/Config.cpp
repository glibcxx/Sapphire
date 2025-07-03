#include "Config.h"

namespace sapphire::inline config {

    ConfigManager::ConfigManager() :
        mConfigDir(sapphire::SapphireModuleInfo::get().sapphireHome / "configs") {
        if (!std::filesystem::exists(mConfigDir)) {
            std::filesystem::create_directories(mConfigDir);
        }
    }

    ConfigManager &ConfigManager::getInstance() {
        static ConfigManager instance;
        return instance;
    }

} // namespace sapphire::inline config