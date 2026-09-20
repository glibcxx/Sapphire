#pragma once

#include <string>

extern "C" {
__declspec(dllimport) int __stdcall WideCharToMultiByte(
    unsigned int   CodePage,
    unsigned long  dwFlags,
    const wchar_t *lpWideCharStr,
    int            cchWideChar,
    char          *lpMultiByteStr,
    int            cbMultiByte,
    const char    *lpDefaultChar,
    int           *lpUsedDefaultChar
);
}

namespace sapphire {

    inline std::string wstringToString(const std::wstring &wstr) {
        int sizeNeeded = WideCharToMultiByte(65001, 0, &wstr[0], (int)wstr.size(), nullptr, 0, nullptr, nullptr);
        if (sizeNeeded <= 0)
            return {};
        std::string strTo(sizeNeeded, 0);
        WideCharToMultiByte(65001, 0, &wstr[0], (int)wstr.size(), &strTo[0], sizeNeeded, nullptr, nullptr);
        return strTo;
    }

    // 如果 str 以 prefix 开头，返回 true
    constexpr bool consumeFront(std::string_view &str, std::string_view prefix) noexcept {
        if (!str.starts_with(prefix)) return false;
        str.remove_prefix(prefix.size());
        return true;
    }

    // 如果 str 以 prefix 开头，返回 true
    constexpr bool consumeFront(std::string_view &str, char prefix) noexcept {
        if (!str.starts_with(prefix)) return false;
        str.remove_prefix(1);
        return true;
    }

    constexpr uint32_t getAutoSenseRadix(std::string_view &str) noexcept {
        if (str.empty()) return 10;
        if (consumeFront(str, "0x") || consumeFront(str, "0X")) return 16;
        if (consumeFront(str, "0b") || consumeFront(str, "0B")) return 2;
        if (consumeFront(str, "0o")) return 8;

        if (str[0] == '0' && str.size() > 1 && std::isdigit(str[1])) {
            str.remove_prefix(1);
            return 8;
        }

        return 10;
    }

    // 如有错误，返回 true
    constexpr bool consumeUnsignedInteger(std::string_view &str, uint32_t radix, unsigned long long &result) noexcept {
        if (radix == 0) radix = getAutoSenseRadix(str);
        if (str.empty()) return true;

        std::string_view str2 = str;
        result = 0;
        while (!str2.empty()) {
            uint8_t charVal;
            if (str2[0] >= '0' && str2[0] <= '9')
                charVal = str2[0] - '0';
            else if (str2[0] >= 'a' && str2[0] <= 'z')
                charVal = str2[0] - 'a' + 10;
            else if (str2[0] >= 'A' && str2[0] <= 'Z')
                charVal = str2[0] - 'A' + 10;
            else
                break;
            if (charVal >= radix) break;
            size_t prevResult = result;
            result = result * radix + charVal;
            if (result / radix < prevResult) return true;
            str2 = str2.substr(1);
        }

        if (str.size() == str2.size()) return true;
        str = str2;
        return false;
    }

#ifdef _MSC_VER
#    pragma warning(push)
#    pragma warning(disable : 4146)
#    pragma warning(disable : 4244)
#endif

    // 如有错误，返回 true
    constexpr bool consumeSignedInteger(std::string_view &str, uint32_t radix, long long &result) noexcept {
        unsigned long long unsignedVal;
        if (!str.starts_with("-")) {
            if (consumeUnsignedInteger(str, radix, unsignedVal) || (long long)unsignedVal < 0) return true;
            result = unsignedVal;
            return false;
        }
        std::string_view str2 = str.substr(1);
        if (consumeUnsignedInteger(str2, radix, unsignedVal) || (long long)-unsignedVal > 0) return true;
        str = str2;
        result = -unsignedVal;
        return false;
    }

    // 如有错误，返回 true
    template <typename IntType>
    constexpr bool consumeInteger(std::string_view &str, uint32_t radix, IntType &result) noexcept {
        if constexpr (std::is_signed_v<IntType>) {
            long long signedVal;
            if (consumeSignedInteger(str, radix, signedVal)
                || static_cast<long long>(static_cast<IntType>(signedVal)) != signedVal)
                return true;
            result = signedVal;
        } else {
            unsigned long long unsingedVal;
            if (consumeUnsignedInteger(str, radix, unsingedVal)
                || static_cast<unsigned long long>(static_cast<IntType>(unsingedVal)) != unsingedVal)
                return true;
            result = unsingedVal;
        }
        return false;
    }

#ifdef _MSC_VER
#    pragma warning(pop)
#endif

} // namespace sapphire