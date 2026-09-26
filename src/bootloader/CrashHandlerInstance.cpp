#include "common/debug/CrashHandler.hpp"
#include <filesystem>

HMODULE getCurrentDllHandle() {
    HMODULE hModule = NULL;
    GetModuleHandleEx(
        GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS | GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT,
        (LPCTSTR)getCurrentDllHandle,
        &hModule
    );
    return hModule;
}

std::filesystem::path getCurrentModulePath() {
    wchar_t path[MAX_PATH] = {0};
    GetModuleFileNameW(getCurrentDllHandle(), path, MAX_PATH);
    return std::filesystem::path{path};
}

static std::filesystem::path binDir = []() {
    auto currentPath = getCurrentModulePath();
    return currentPath.parent_path();
}();
static std::filesystem::path outputDir = binDir.parent_path() / "sapphire" / "crash";

static bool crashInfoWriter(std::string_view info) {
    std::chrono::zoned_time zt{std::chrono::current_zone(), std::chrono::system_clock::now()};

    auto   logPath = outputDir / std::format(L"crash_dump_{:%Y.%m.%d-%H_%M_%S}.log", zt);
    HANDLE hFile = CreateFileW(
        logPath.c_str(),
        GENERIC_WRITE,
        FILE_SHARE_READ,
        nullptr,
        CREATE_ALWAYS,
        FILE_ATTRIBUTE_NORMAL,
        nullptr
    );

    if (hFile != INVALID_HANDLE_VALUE) {
        DWORD written;
        WriteFile(hFile, info.data(), static_cast<DWORD>(info.size()), &written, nullptr);
        CloseHandle(hFile);
        return true;
    }
    return false;
}

static sapphire::debug::CrashHandler handler{&crashInfoWriter};