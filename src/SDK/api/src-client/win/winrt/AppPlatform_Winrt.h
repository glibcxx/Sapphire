#pragma once

#include <winrt/windows.ui.core.h>
#include "SDK/api/src/win/AppPlatformWindows.h"

class AppPlatformWinRTDispatcher;

// size: 2008 (1.21.2), 2016 (1.21.50/1.21.60)
class AppPlatform_Winrt : public AppPlatformWindows {
public:
    ;                                                                    // offset: (1.21.50/1.21.60), (1.21.2 = 1.21.50 - 8)
    std::string                                    mUnk1792;             // off+1792
    uint64_t                                       mUnk1824;             // off+1824
    uint32_t                                       mDisplayLogicalDpi;   // off+1832
    float                                          mDisplayScaleFactorX; // off+1836
    float                                          mDisplayScaleFactorY; // off+1840
    bool                                           mUnk1844;             // off+1844
    uint64_t                                       mUnk1848;             // off+1848
    uint64_t                                       mUnk1856;             // off+1856
    uint64_t                                       mUnk1864;             // off+1864
    std::optional<int>                             mUnk1872;             // off+1872
    uint32_t                                       mUnk1880;             // off+1880
    std::unique_ptr<AppPlatformWinRTDispatcher>    mPlatformDispatcher;  // off+1888
    std::string                                    mUnk1896;             // off+1896
    uint64_t                                       _fill1928[8];         // off+1928
    std::shared_ptr<std::aligned_storage_t<96, 8>> mUnk1992;             // off+1992
    uint64_t                                       mUnk2008;             // off+2008

    SDK_API AppPlatform_Winrt *ctor(winrt::Windows::UI::Core::CoreWindow coreWindow);

    SDK_API void update();
};
#if MC_VERSION == v1_21_2
static_assert(sizeof(AppPlatform_Winrt) == 2008);
#elif MC_VERSION == v1_21_50 || MC_VERSION == v1_21_60
static_assert(sizeof(AppPlatform_Winrt) == 2016);
#endif