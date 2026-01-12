#pragma once

#include "SDK/api/sapphire/logger/Logger.h"
#include "SDK/api/sapphire/IMod.h"

namespace sapphire::example {

    class BuiltinMod : public sapphire::IMod {
        sapphire::Logger mLog{"BuiltinMod"};

    public:
        virtual void onLoaded() override;

        virtual void onUnload() override;

        virtual void onInit(const sapphire::ModInitContext &context) override;

        virtual void onUnInit() override;

        static sapphire::Logger &getLogger();
    };

} // namespace sapphire::example
