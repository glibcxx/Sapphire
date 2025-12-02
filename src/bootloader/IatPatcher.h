#pragma once

#include <windows.h>
#include <string>
#include <unordered_map>
#include <vector>

#include "macros/Macros.h"

namespace sapphire::bootloader {

    class IatPatcher {
    public:
        using ApiMap = std::unordered_map<std::string, uintptr_t>;

        SPHR_API void patchModule(HMODULE hModule, const ApiMap &apiMap);

        IatPatcher(const std::string &bedrockSigSourceDllName);
        ~IatPatcher() = default;

        IatPatcher(const IatPatcher &) = delete;
        IatPatcher &operator=(const IatPatcher &) = delete;

    private:
        bool patchModuleInternal(HMODULE hModuleToPatch, const ApiMap &apiMap);

        std::string mBedrockSigSourceDllName;
    };

} // namespace sapphire::bootloader