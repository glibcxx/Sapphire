#pragma once

#include <string>
#include <unordered_map>
#include "common/sys/MiniWindows.h"

namespace sapphire::ipc {
    class Client;
}

namespace sapphire::bootloader {

    class IatPatcher {
    public:
        using ApiMap = std::unordered_map<std::string, uintptr_t>;

        bool patchModule(sys::win::hmodule_t hModule, const ApiMap &apiMap, const ApiMap &dataApiMap);

        IatPatcher(const std::string &bedrockSigSourceDllName, ipc::Client &IPCClient);
        ~IatPatcher() = default;

        IatPatcher(const IatPatcher &) = delete;
        IatPatcher &operator=(const IatPatcher &) = delete;

    private:
        bool patchModuleInternal(sys::win::hmodule_t hModuleToPatch, const ApiMap &apiMap, const ApiMap &dataApiMap);

        std::string  mBedrockSigSourceDllName;
        ipc::Client &mIPCClient;
    };

} // namespace sapphire::bootloader