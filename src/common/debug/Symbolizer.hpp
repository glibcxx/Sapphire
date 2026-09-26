#pragma once

#include <algorithm>
#include <cassert>
#include <cstddef>
#include <format>
#include <iterator>
#include <span>
#include <string>
#include <string_view>
#include <windows.h>
#include <dbghelp.h>

#pragma comment(lib, "dbghelp.lib")

namespace sapphire::debug {

    inline constexpr size_t MAX_MODULE_NAME_LENGTH = 32;
    inline constexpr size_t MAX_FUNCTION_NAME_LENGTH = 256;
    inline constexpr size_t MAX_FILE_NAME_LENGTH = MAX_PATH;

    struct SymbolInfo {
        uintptr_t mAddress = 0;

        // 1. 模块信息
        char      mModuleName[MAX_MODULE_NAME_LENGTH] = {};
        uintptr_t mModuleBase = 0;
        uintptr_t mModuleOffset = 0; // RVA

        // 2. 函数/符号信息
        char      mFunctionName[MAX_FUNCTION_NAME_LENGTH] = {};
        uintptr_t mFunctionOffset = 0;

        // 3. 源码行号
        char     mFileName[MAX_FILE_NAME_LENGTH] = {};
        uint32_t mLineNumber = 0;
        uint32_t mLineOffset = 0;

        // 有效性标志
        bool mHasModule = false;
        bool mHasSymbol = false;
        bool mHasLine = false;
    };

    class Symbolizer {
    public:
        enum class DemangleOption {
            Complete,
            NameOnly,
            NoCallingConv
        };

        static Symbolizer &instance() noexcept {
            static Symbolizer sInstance;
            return sInstance;
        }

        Symbolizer(const Symbolizer &) = delete;
        Symbolizer &operator=(const Symbolizer &) = delete;

        [[nodiscard]] bool isInitialized() const noexcept {
            return mInitialized;
        }

        bool resolve(uintptr_t address, SymbolInfo &outInfo) noexcept {
            outInfo = SymbolInfo{};
            outInfo.mAddress = address;

            if (!mInitialized) {
                return false;
            }

            IMAGEHLP_MODULE64 modInfo{};
            modInfo.SizeOfStruct = sizeof(IMAGEHLP_MODULE64);
            if (SymGetModuleInfo64(mProcessHandle, address, &modInfo)) {
                outInfo.mHasModule = true;
                outInfo.mModuleBase = static_cast<uintptr_t>(modInfo.BaseOfImage);
                outInfo.mModuleOffset = address - outInfo.mModuleBase;
                memcpy(outInfo.mModuleName, modInfo.ModuleName, 32);
            }

            alignas(SYMBOL_INFO) char symBuffer[sizeof(SYMBOL_INFO) + MAX_SYM_NAME * sizeof(char)] = {};
            auto                     *pSymbol = reinterpret_cast<PSYMBOL_INFO>(symBuffer);
            pSymbol->SizeOfStruct = sizeof(SYMBOL_INFO);
            pSymbol->MaxNameLen = MAX_SYM_NAME;

            DWORD64 symDisplacement = 0;
            if (SymFromAddr(mProcessHandle, address, &symDisplacement, pSymbol)) {
                outInfo.mHasSymbol = true;
                outInfo.mFunctionOffset = static_cast<uintptr_t>(symDisplacement);
                memcpy(outInfo.mFunctionName, pSymbol->Name, std::min<size_t>(256, pSymbol->NameLen));
            }

            IMAGEHLP_LINE64 lineInfo{};
            lineInfo.SizeOfStruct = sizeof(IMAGEHLP_LINE64);
            DWORD lineDisplacement = 0;

            if (SymGetLineFromAddr64(mProcessHandle, address, &lineDisplacement, &lineInfo)) {
                outInfo.mHasLine = true;
                outInfo.mLineNumber = lineInfo.LineNumber;
                outInfo.mLineOffset = lineDisplacement;
                copyString(outInfo.mFileName, lineInfo.FileName);
            }

            return outInfo.mHasModule || outInfo.mHasSymbol;
        }

        void resolveBatch(std::span<const uintptr_t> addresses, std::span<SymbolInfo> outInfos) noexcept {
            const size_t count = (std::min)(addresses.size(), outInfos.size());
            for (size_t i = 0; i < count; ++i) {
                resolve(addresses[i], outInfos[i]);
            }
        }

        [[nodiscard]] std::string format(const SymbolInfo &info) const {
            std::string result;
            auto        out = std::back_inserter(result);

            if (info.mHasSymbol && info.mHasLine) {
                std::format_to(out, "0x{:016X} {}!{}+0x{:X} [{}:{}]", info.mAddress, info.mHasModule ? info.mModuleName : "??", info.mFunctionName, info.mFunctionOffset, info.mFileName, info.mLineNumber);
            } else if (info.mHasSymbol) {
                std::format_to(out, "0x{:016X} {}!{}+0x{:X}", info.mAddress, info.mHasModule ? info.mModuleName : "??", info.mFunctionName, info.mFunctionOffset);
            } else if (info.mHasModule) {
                std::format_to(out, "0x{:016X} {}+0x{:X}", info.mAddress, info.mModuleName, info.mModuleOffset);
            } else {
                std::format_to(out, "0x{:016X} ??", info.mAddress);
            }

            return result;
        }

        [[nodiscard]] std::string resolveAndFormat(uintptr_t address) {
            SymbolInfo info;
            resolve(address, info);
            return format(info);
        }

        static bool demangle(std::string_view mangledName, std::span<char> outBuffer, DemangleOption option = DemangleOption::Complete) noexcept {
            if (outBuffer.empty() || mangledName.empty()) {
                return false;
            }

            // 调用方需保证传入的 string_view 内存以 '\0' 结尾
            assert(mangledName.data()[mangledName.size()] == '\0' && "mangledName must be null-terminated for UnDecorateSymbolName");

            DWORD flags = UNDNAME_COMPLETE;
            switch (option) {
            case DemangleOption::NameOnly:
                flags = UNDNAME_NAME_ONLY;
                break;
            case DemangleOption::NoCallingConv:
                flags = UNDNAME_COMPLETE | UNDNAME_NO_ALLOCATION_MODEL | UNDNAME_NO_ALLOCATION_LANGUAGE;
                break;
            default:
                break;
            }

            return UnDecorateSymbolName(mangledName.data(), outBuffer.data(), static_cast<DWORD>(outBuffer.size()), flags) > 0;
        }

        [[nodiscard]] static std::string demangle(std::string_view mangledName, DemangleOption option = DemangleOption::Complete) {
            std::string result(MAX_FUNCTION_NAME_LENGTH, '\0');
            if (demangle(mangledName, std::span<char>(result.data(), result.size()), option)) {
                result.resize(std::strlen(result.c_str()));
                return result;
            }
            return std::string(mangledName);
        }

        bool addSearchPath(std::wstring_view path) noexcept {
            if (!mInitialized || path.empty()) {
                return false;
            }
            if (mSearchPath.find(path) != std::string::npos) return true;
            if (!mSearchPath.empty())
                mSearchPath.push_back(L';');
            mSearchPath.append(path);
            return SymSetSearchPathW(mProcessHandle, mSearchPath.data()) == TRUE;
        }

        void refreshModules() noexcept {
            if (mInitialized) {
                SymRefreshModuleList(mProcessHandle);
            }
        }

    private:
        Symbolizer() noexcept {
            mProcessHandle = GetCurrentProcess();

            SymSetOptions(SYMOPT_DEFERRED_LOADS | SYMOPT_LOAD_LINES | SYMOPT_UNDNAME);
            if (SymInitialize(mProcessHandle, nullptr, TRUE)) {
                mInitialized = true;

                wchar_t tmp[2048];
                SymGetSearchPathW(mProcessHandle, tmp, 2048);
                mSearchPath = tmp;
            }
        }

        ~Symbolizer() noexcept {
            if (mInitialized) {
                SymCleanup(mProcessHandle);
                mInitialized = false;
            }
        }

        template <size_t N>
        static void copyString(char (&dest)[N], const char *src) noexcept {
            if (!src) {
                dest[0] = '\0';
                return;
            }
            std::string_view sv(src);
            const size_t     copyCount = std::min(sv.size(), N - 1);
            std::copy_n(sv.data(), copyCount, dest);
            dest[copyCount] = '\0';
        }

        HANDLE       mProcessHandle = nullptr;
        bool         mInitialized = false;
        std::wstring mSearchPath;
    };

} // namespace sapphire::debug