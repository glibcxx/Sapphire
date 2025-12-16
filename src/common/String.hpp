#pragma once

#include <string>
#include <Windows.h>

namespace util {
    inline std::string wstringToString(const std::wstring &wstr) {
        int sizeNeeded = WideCharToMultiByte(CP_UTF8, 0, &wstr[0], (int)wstr.size(), NULL, 0, NULL, NULL);
        if (sizeNeeded <= 0)
            return {};
        std::string strTo(sizeNeeded, 0);
        WideCharToMultiByte(CP_UTF8, 0, &wstr[0], (int)wstr.size(), &strTo[0], sizeNeeded, NULL, NULL);
        return strTo;
    }
} // namespace util