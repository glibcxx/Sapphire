#pragma once

#include "macros/Macros.h"

namespace sapphire {

    class IPlugin {
    public:
        SDK_API IPlugin();

        virtual ~IPlugin() = default;

        virtual void init() = 0;

        virtual void uninit() = 0;
    };

} // namespace sapphire