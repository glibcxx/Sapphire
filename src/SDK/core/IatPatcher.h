#pragma once

#include <windows.h>
#include <string>
#include <unordered_map>
#include <vector>
#include "macros/Macros.h"

namespace sapphire::core {

    class PluginManager;

    class IatPatcher {
    public:
        using ApiMap = std::unordered_map<std::string_view, uintptr_t>;

        void patchModule(HMODULE hModule, const ApiMap &apiMap);

        IatPatcher(const std::string &sapphireCoreDllName);
        ~IatPatcher() = default;

        IatPatcher(const IatPatcher &) = delete;
        IatPatcher &operator=(const IatPatcher &) = delete;

    private:
        bool patchModuleInternal(HMODULE hModuleToPatch, const ApiMap &apiMap);

        std::vector<std::string> mDllNames;
    };

} // namespace sapphire::core