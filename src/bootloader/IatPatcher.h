#pragma once

#include <windows.h>
#include <string>
#include <unordered_map>
#include <vector>

#include "util/IPC/Client.h"

namespace sapphire::bootloader {

    class IatPatcher {
    public:
        using ApiMap = std::unordered_map<std::string, uintptr_t>;

        bool patchModule(HMODULE hModule, const ApiMap &apiMap);

        IatPatcher(const std::string &bedrockSigSourceDllName, ipc::Client &IPCClient);
        ~IatPatcher() = default;

        IatPatcher(const IatPatcher &) = delete;
        IatPatcher &operator=(const IatPatcher &) = delete;

    private:
        bool patchModuleInternal(HMODULE hModuleToPatch, const ApiMap &apiMap);

        std::string  mBedrockSigSourceDllName;
        ipc::Client &mIPCClient;
    };

} // namespace sapphire::bootloader