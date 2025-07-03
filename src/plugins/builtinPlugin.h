#pragma once

#include "SDK/core/Core.h"

class BuiltinPlugin : public sapphire::IPlugin {
public:
    virtual void init() override;

    virtual void uninit() override;
};