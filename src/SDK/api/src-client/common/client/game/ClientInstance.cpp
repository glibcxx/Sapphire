#include "ClientInstance.h"

#include "SDK/core/Core.h"
#include "SDK/core/ApiManager.h"
#include "SDK/api/sapphire/hook/Hook.h"
#include "util/ScopeGuard.hpp"

#include "SDK/api/src/common/certificates/Certificate.h"
#include "SDK/api/src-client/common/client/network/LegacyClientNetworkHandler.h"

void *const *ClientInstance::__vftable0 = reinterpret_cast<void *const *>(
    sapphire::Core::getInstance().getImagebase()
#if MC_VERSION == v1_21_2
    + 0x4CBCB68
#elif MC_VERSION == v1_21_50
    + 0x6382D58
#elif MC_VERSION == v1_21_60
    + 0x6C34DF8
#endif
);

void *const *ClientInstance::__vftable1 = reinterpret_cast<void *const *>(
    sapphire::Core::getInstance().getImagebase()
#if MC_VERSION == v1_21_2
    + 0x4CBD920
#elif MC_VERSION == v1_21_50
    + 0x6383B60
#elif MC_VERSION == v1_21_60
    + 0x6C34D78
#endif
);

void *const *ClientInstance::__vftable2 = reinterpret_cast<void *const *>(
    sapphire::Core::getInstance().getImagebase()
#if MC_VERSION == v1_21_2
    + 0x4CBD948
#elif MC_VERSION == v1_21_50
    + 0x6382CE0
#elif MC_VERSION == v1_21_60
    + 0x6C34DA0
#endif
);

void *const *ClientInstance::__vftable3 = reinterpret_cast<void *const *>(
    sapphire::Core::getInstance().getImagebase()
#if MC_VERSION == v1_21_2
    + 0x4CBD9A0
#elif MC_VERSION == v1_21_50
    + 0x6382D38
#elif MC_VERSION == v1_21_60
    + 0x6C36A38
#endif
);

ClientInstance *ClientInstance::primaryClientInstance = nullptr;

RenderCameraComponent *ClientInstance::getRenderCameraComponent() const {
    using Hook = sapphire::ApiLoader<
        "\xE8\x00\x00\x00\x00\x48\x85\xC0\x74\x00\xF3\x0F\x10\x58\x00\xF3\x0F\x10\x05"_sig,
        &ClientInstance::getRenderCameraComponent,
        [](uintptr_t addr) {
            return memory::deRef(addr, memory::AsmOperation::CALL);
        }>;
    return (this->*Hook::origin)();
}

bool ClientInstance::getRenderPlayerModel() const {
    using Hook = sapphire::ApiLoader<
#if MC_VERSION == v1_21_2
        "\x48\x89\x5C\x24\x00\x48\x89\x74\x24\x00\x48\x89\x7C\x24\x00\x55\x41\x56\x41\x57\x48\x8D\x6C\x24\x00\x48\x81\xEC\x00\x00\x00\x00\x48\x8B\x05\x00\x00\x00\x00\x48\x33\xC4\x48\x89\x45\x00\x0F\x57\xC0"_sig,
#elif MC_VERSION == v1_21_50
        "\x48\x89\x5C\x24\x00\x48\x89\x74\x24\x00\x55\x57\x41\x56\x48\x8D\x6C\x24\x00\x48\x81\xEC\x00\x00\x00\x00\x0F\x29\xB4\x24\x00\x00\x00\x00\x48\x8B\x05\x00\x00\x00\x00\x48\x33\xC4\x48\x89\x45\x00\x0F\x57\xC0"_sig,
#elif MC_VERSION == v1_21_60
        "\x48\x8B\xC4\x48\x89\x58\x00\x48\x89\x70\x00\x48\x89\x78\x00\x55\x41\x56\x41\x57\x48\x8D\x68\x00\x48\x81\xEC\x00\x00\x00\x00\x0F\x29\x70\x00\x48\x8B\x05\x00\x00\x00\x00\x48\x33\xC4\x48\x89\x45\x00\x4C\x8B\xF1"_sig,
#endif
        &ClientInstance::getRenderPlayerModel>;
    return (this->*Hook::origin)();
}

std::unique_ptr<LegacyClientNetworkHandler> ClientInstance::_createNetworkHandler(std::unique_ptr<Certificate> cert) {
    using Hook = sapphire::ApiLoader<
#if MC_VERSION == v1_21_50
        "\xE8\x00\x00\x00\x00\x90\x48\x8B\x10\x48\x89\x28\x48\x89\x54\x24\x00\x48\x8D\x54\x24\x00\x48\x8B\xCB"_sig,
#endif
        &ClientInstance::_createNetworkHandler,
        sapphire::deRefCall>;
    return (this->*Hook::origin)(std::move(cert));
}
