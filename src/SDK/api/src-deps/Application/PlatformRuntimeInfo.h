#pragma once

#include "SDK/api/src-deps/Core/File/Path.h"
#include "SDK/api/src-deps/Core/Platform/PlatformTypes.h"

namespace Bedrock {

    // size: 728
    class PlatformRuntimeInfo {
    public:
        std::string          mDeviceModelName;                 // off+8
        std::string          mOSVersion;                       // off+40
        std::string          mCPUType;                         // off+72
        std::string          mCPUName;                         // off+104
        std::string          mCPUFeatures;                     // off+136
        std::string          mGPUDriverVersion;                // off+168
        std::string          mSecureId;                        // off+200
        std::string          mSerial;                          // off+232
        std::string          mBoard;                           // off+264
        std::string          mInstallerPackageName;            // off+296
        std::string          mRegion;                          // off+328
        PlatformType         mPlatformType;                    // off+360
        size_t               mCachedFreeStorageSpace_Internal; // off+368
        size_t               mCachedFreeStorageSpace_External; // off+376
        size_t               mCachedFreeStorageSpace_Cloud;    // off+384
        size_t               mTotalPhysicalMemory;             // off+392
        size_t               mTotalVirtualMemory;              // off+400
        size_t               mUsedMemory;                      // off+408
        size_t               mPhysicalMemorySize;              // off+416
        uint32_t             mOptimalLDBSize;                  // off+424
        float                mWidth;                           // off+428
        float                mHeight;                          // off+432
        float                mDPI;                             // off+436
        DisplayOrientation   mOrientation;                     // off+440
        int                  mSignaturesHash;                  // off+444
        bool                 mGraphicsTearingSupport;          // off+448
        bool                 mAllowSplitScreen;                // off+449
        bool                 mSupportsMSAA;                    // off+450
        bool                 mHasFastAlphaTest;                // off+451
        bool                 mSupportsVibration;               // off+452
        bool                 mSupportsTextToSpeech;            // off+453
        bool                 mSupportsClipboard;               // off+454
        bool                 mSupportsFilePicking;             // off+455
        bool                 mAllowContentLogWrite;            // off+456
        bool                 mIsRooted;                        // off+457
        bool                 mCanSelfTerminate;                // off+458
        bool                 mCanLaunchUri;                    // off+459
        uint8_t              mCoreCount;                       // off+460
        uint8_t              mThreadCount;                     // off+461
        uint8_t              mHighPerfThreadCount;             // off+462
        uint8_t              mProcessorGrade;                  // off+463
        uint8_t              mGraphicsGrade;                   // off+464
        uint8_t              mMemoryGrade;                     // off+465
        uint8_t              mStorageGrade;                    // off+466
        uint8_t              mPowerSupplyGrade;                // off+467
        uint32_t             mCPUBits;                         // off+468
        Core::HeapPathBuffer mAssetStoragePath;                // off+472
        Core::HeapPathBuffer mCurrentStoragePath;              // off+504
        Core::HeapPathBuffer mExternalStoragePath;             // off+536
        Core::HeapPathBuffer mInternalStoragePath;             // off+568
        Core::HeapPathBuffer mLoggingPath;                     // off+600
        Core::HeapPathBuffer mPackagePath;                     // off+632
        Core::HeapPathBuffer mUserDataPath;                    // off+664
        Core::HeapPathBuffer mCacheStoragePath;                // off+696

        PlatformRuntimeInfo *ctor(); // \xE8\x00\x00\x00\x00\x48\x8B\xCF\xE8\x00\x00\x00\x00\x48\x89\x03\x48\x8B\xC3 (1.21.2)
                                     // \xE8\x00\x00\x00\x00\xEB\x00\x48\x8B\xC6\x48\x8B\x4F (1.21.50)
                                     // \xE8\x00\x00\x00\x00\x48\x8B\x8F\x00\x00\x00\x00\x48\x89\x87 (1.21.60)

        // vtb+0
        virtual ~PlatformRuntimeInfo();
    };

} // namespace Bedrock