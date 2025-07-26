#pragma once

#include "SDK/core/IPlugin.h"

class BuiltinPlugin : public sapphire::IPlugin {
public:
    virtual void init() override;

    virtual void uninit() override;
};