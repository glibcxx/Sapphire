#include "Config.h"

#include "SDK/api/sapphire/platform/Environment.h"
#include "SDK/core/Runtime.h"

namespace sapphire {

    ConfigManager::ConfigManager() :
        mConfigDir(sapphire::platform::Environment::getInstance().getSapphireHomePath() / "configs") {
        if (!std::filesystem::exists(mConfigDir)) {
            std::filesystem::create_directories(mConfigDir);
        }
    }

    ConfigManager &ConfigManager::getInstance() {
        static ConfigManager instance;
        return instance;
    }

} // namespace sapphire