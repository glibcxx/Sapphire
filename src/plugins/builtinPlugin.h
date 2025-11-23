#pragma once

#include "SDK/api/sapphire/logger/Logger.h"
#include "SDK/api/sapphire/IPlugin.h"

class BuiltinPlugin : public sapphire::IPlugin {
    sapphire::Logger mLog{"BuiltinPlugin"};

public:
    virtual void onLoaded() override;

    virtual void onUnload() override;

    virtual void onInit(const sapphire::PluginInitContext &context) override;

    virtual void onUnInit() override;

    static sapphire::Logger &getLogger();
};