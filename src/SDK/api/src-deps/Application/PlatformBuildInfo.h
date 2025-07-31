#pragma once

#include "SDK/api/src-deps/Core/File/Path.h"

namespace Bedrock {

    // size: 200
    class PlatformBuildInfo {
    public:
        std::string          mInstallerPackageName;           // off+0
        Core::HeapPathBuffer mPlatformTempPath;               // off+32
        Core::HeapPathBuffer mOnDiskScratchPath;              // off+64
        Core::HeapPathBuffer mDataUrl;                        // off+96
        Core::HeapPathBuffer mAltDataUrl;                     // off+128
        std::string          mFeedbackURL;                    // off+160
        bool                 mHasBuyButtonWhenLicenseInvalid; // off+192
    };

} // namespace Bedrock