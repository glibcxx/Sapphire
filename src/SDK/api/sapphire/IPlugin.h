#pragma once

#include "macros/Macros.h"

class MinecraftGame;
class IClientInstance;

namespace sapphire {

    struct PluginInitContext {
        MinecraftGame   &mMinecrftGame;
        IClientInstance &mClientInstance;
    };

    class IPlugin {
    public:
        SPHR_API IPlugin();

        virtual ~IPlugin() = default;

        virtual void onLoaded() = 0;

        virtual void onUnload() = 0;

        virtual void onInit(const PluginInitContext &context) = 0;

        virtual void onUnInit() = 0;
    };

} // namespace sapphire