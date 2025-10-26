#pragma once

#include "macros/Macros.h"

namespace sapphire {

    class IPlugin {
    public:
        SPHR_API IPlugin();

        virtual ~IPlugin() = default;

        virtual void onLoaded() = 0;

        virtual void onUnload() = 0;

        virtual void onInit() = 0;

        virtual void onUnInit() = 0;
    };

} // namespace sapphire