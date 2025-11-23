#include "IPlugin.h"

#include "SDK/core/Runtime.h"

namespace sapphire {

    IPlugin::IPlugin() {
        core::Runtime::getInstance().getPluginManager().registerPlugin(*this);
    }

} // namespace sapphire
