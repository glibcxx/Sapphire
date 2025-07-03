#include "IPlugin.h"

#include "Core.h"

namespace sapphire {

    IPlugin::IPlugin() {
        Core::getInstance().getPluginManager().registerPlugin(*this);
    }

} // namespace sapphire
