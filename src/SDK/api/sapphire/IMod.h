#pragma once

#include "macros/Macros.h"

class MinecraftGame;
class IClientInstance;

namespace sapphire {

    struct ModInitContext {
        MinecraftGame   &mMinecrftGame;
        IClientInstance &mClientInstance;
    };

    class IMod {
    public:
        virtual ~IMod() = default;

        virtual void onLoaded() = 0;

        virtual void onUnload() = 0;

        virtual void onInit(const ModInitContext &context) = 0;

        virtual void onUnInit() = 0;
    };

} // namespace sapphire

#define SPHR_REGISTRY_MOD_2(ModTypeName, varName)                 \
    static ModTypeName                       varName{};           \
    extern "C" SPHR_EXPORT ::sapphire::IMod *sapphireModEntry() { \
        return &varName;                                          \
    }

#define SPHR_REGISTRY_MOD_1(ModTypeName) SPHR_REGISTRY_MOD_2(ModTypeName, SPHR_CONCAT(modInstance_, __LINE__))

#define SPHR_REGISTRY_MOD_GET_MACRO(_1, _2, NAME, ...) NAME

#define SPHR_REGISTRY_MOD(...) \
    SPHR_REGISTRY_MOD_GET_MACRO(__VA_ARGS__, SPHR_REGISTRY_MOD_2, SPHR_REGISTRY_MOD_1)(__VA_ARGS__)
