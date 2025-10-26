#pragma once

#include "SDK/core/IPlugin.h"

class BuiltinPlugin : public sapphire::IPlugin {
public:
    virtual void onLoaded() override;

    virtual void onUnload() override;

    virtual void onInit() override;

    virtual void onUnInit() override;
};