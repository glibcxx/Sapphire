#pragma once

#include "IAppPlatform.h"
#include "SDK/core/SymbolResolver.h"
#include "SDK/api/src/common/AppPlatformListener.h"
#include "SDK/api/src-deps/Core/SecureStorage/SecureStorageKey.h"
#include "SDK/api/src-deps/Core/Utility/ServiceLocator.h"
#include "SDK/api/src-deps/Core/Utility/Subject.h"
#include "SDK/api/src-deps/Core/NetworkChangeObserver.h"
#include "SDK/api/src-deps/Core/Platform/AppLifecycleContext.h"
#include "SDK/api/src-deps/Core/Platform/PlatformTypes.h"
#include "SDK/api/src-deps/Core/File/FileSystem/FileInterface.h"
#include "SDK/api/src-deps/Core/Utility/ServiceRegistrationToken.h"

class UriListener;
class FeatureToggles;
namespace Bedrock {
    class CommonPlatform;
    class SignalReceiver;
    class PlatformRuntimeInfo;
    class PlatformBuildInfo;
} // namespace Bedrock

enum class StoragePermissionResult : uint8_t {
    PermissionGranted = 0,
    PermissionDenied = 1,
};

using StringVector = std::vector<std::string>;

using MemoryTier = int;

// size: 1032 (1.21.2), 1040 (1.21.50/1.21.60)
class AppPlatform : public IAppPlatform, public ISecureStorageKeySystem {
public:
    static constexpr int64_t UNDETERMINED_MEMORY = 0;

    using Listener = AppPlatformListener;

    bool                                          mRequestedRestart; // off+32
    uint64_t                                      mUnk40;            // off+40
    std::unique_ptr<std::aligned_storage_t<8, 8>> mUnk48;            // off+48

    bool                                                           mPointerFocusLost;                       // off+56
    bool                                                           mKeyboardVisible;                        // off+57
    bool                                                           mOnInitUriListenerRegDone;               // off+58
    uint32_t                                                       mUnk60;                                  // off+60
    std::multimap<float, Listener *>                               mListeners;                              // off+64
    Core::Observer<NetworkChangeObserver, std::mutex>::SubjectType mNetworkChangeSubject;                   // off+80
    uint8_t                                                        mUnk184;                                 // off+184
    uint64_t                                                       mUnk192;                                 // off+192
    std::unordered_multimap<std::string, UriListener *>            mUriListeners;                           // off+200
    std::function<void(StoragePermissionResult)>                   mStoragePermissionRequestResultCallback; // off+264
    AppLifecycleContext                                            mAppLifecycleContext;                    // off+328
    AppFocusState                                                  mFocusState;                             // off+332
    std::mutex                                                     mShareLock;                              // off+336
    std::string                                                    mShareTitle;                             // off+416
    std::string                                                    mShareText;                              // off+448
    std::string                                                    mShareUri;                               // off+480
    ARVRPlatform                                                   mHMDPlatform;                            // off+512
    std::unique_ptr<IFileAccess>                                   mDefaultFileAccess;                      // off+520
    std::unique_ptr<IFileAccess>                                   mPackageFileAccess;                      // off+528
    size_t                                                         mMaximumMemoryUsage;                     // off+536
    Bedrock::CommonPlatform                                       *mCommonPlatformShim;                     // off+544
    bool                                                           mMockMultiplayerActive;                  // off+552
    std::string                                                    mLocale;                                 // off+560
    double                                                         mActiveSeconds;                          // off+592
    double                                                         mLastActiveTime;                         // off+600
    bool                                                           mAppIsActive = true;                     // off+608
    uint64_t                                                       mUnk616;                                 // off+616

    std::atomic<bool>                             mTerminating;                         // off+624
    std::atomic<bool>                             mSuspended;                           // off+625
    int32_t                                       mForcedDpi;                           // off+628
    UIScalingRules                                mForcedUIScalingRules;                // off+632
    bool                                          mForceUIScalingRules = true;          // off+636
    bool                                          mShowLostFocusToasts = false;         // off+637
    bool                                          mAllowLostFocusToasts = false;        // off+638
    bool                                          mAreThreadsFrozen = false;            // off+639
    bool                                          mIsLowMemoryDevice = false;           // off+640
    bool                                          mUnk641 = false;                      // off+641
    bool                                          mUnk642 = false;                      // off+642
    Core::HeapPathBuffer                          mScratchPath;                         // off+648
    std::mutex                                    mScratchPathMutex;                    // off+680
    std::string                                   mDeviceID;                            // off+760
    std::recursive_mutex                          mListenerLock;                        // off+792
    std::unique_ptr<Bedrock::SignalReceiver>      mSignalRcvr;                          // off+872
    std::unique_ptr<Bedrock::PlatformRuntimeInfo> mPlatformRuntimeInfo;                 // off+880
    std::unique_ptr<Bedrock::PlatformBuildInfo>   mPlatformBuildInfo;                   // off+888
    uint64_t                                      _fill896[12];                         // off+896
    ServiceRegistrationToken<AppPlatform>         mAppPlatformServiceRegistrationToken; // off+992
    uint32_t                                      mCPUBoostCounter;                     // off+1000
    uint64_t                                      mUnk1008;                             // off+1008
    uint64_t                                      mUnk1016;                             // off+1016

#if MC_VERSION == v1_21_2
    uint64_t mUnk1024; // off+1024
#elif MC_VERSION == v1_21_50 || MC_VERSION == v1_21_60
    std::unique_ptr<std::aligned_storage_t<128, 8>> mUnk1024; // off+1024
    std::unique_ptr<std::aligned_storage_t<128, 8>> mUnk1032; // off+1032
#endif

    SAPPHIRE_API("1.21.2", "\x48\x89\x5C\x24\x00\x48\x89\x74\x24\x00\x48\x89\x7C\x24\x00\x55\x41\x54\x41\x55\x41\x56\x41\x57\x48\x8D\x6C\x24\x00\x48\x81\xEC\x00\x00\x00\x00\x48\x8B\x05\x00\x00\x00\x00\x48\x33\xC4\x48\x89\x45\x00\x48\x8B\xF1\x45\x33\xF6")
    SAPPHIRE_API("1.21.50", "\x48\x89\x5C\x24\x00\x48\x89\x74\x24\x00\x48\x89\x7C\x24\x00\x55\x41\x54\x41\x55\x41\x56\x41\x57\x48\x8D\xAC\x24\x00\x00\x00\x00\x48\x81\xEC\x00\x00\x00\x00\x48\x8B\x05\x00\x00\x00\x00\x48\x33\xC4\x48\x89\x85\x00\x00\x00\x00\x48\x8B\xF9\x45\x33\xE4\x45\x8D\x6C\x24")
    SAPPHIRE_API("1.21.60", "\x48\x89\x5C\x24\x00\x48\x89\x74\x24\x00\x48\x89\x7C\x24\x00\x55\x41\x54\x41\x55\x41\x56\x41\x57\x48\x8D\xAC\x24\x00\x00\x00\x00\x48\x81\xEC\x00\x00\x00\x00\x48\x8B\x05\x00\x00\x00\x00\x48\x33\xC4\x48\x89\x85\x00\x00\x00\x00\x48\x8B\xF9\x45\x33\xED\x41\xBE")
    SDK_API /*virtual*/ void initialize();

    SAPPHIRE_API("1.21.2", "\x48\x89\x5C\x24\x00\x48\x89\x74\x24\x00\x57\x48\x83\xEC\x00\x0F\x29\x74\x24\x00\x0F\x28\xF2")
    SAPPHIRE_API("1.21.50,1.21.60", "\x48\x89\x5C\x24\x00\x48\x89\x6C\x24\x00\x56\x57\x41\x56\x48\x83\xEC\x00\x0F\x29\x74\x24\x00\x0F\x28\xF2\x48\x8B\xEA\x48\x8B\xF1")
    SDK_API /*virtual*/ void addListener(Listener *l, float priority);

    SAPPHIRE_API("1.21.2", "\x48\x89\x5C\x24\x00\x48\x89\x74\x24\x00\x57\x48\x83\xEC\x00\x48\x8B\xF9\x48\x8B\xF2\x48\x81\xC1\x00\x00\x00\x00\xFF\x15\x00\x00\x00\x00\x85\xC0\x74\x00\x8B\xC8\xFF\x15\x00\x00\x00\x00\xCC\x48\x8B\x47")
    SAPPHIRE_API("1.21.50,1.21.60", "\x48\x89\x5C\x24\x00\x48\x89\x74\x24\x00\x57\x48\x83\xEC\x00\x48\x8B\xF2\x48\x8B\xF9\x48\x81\xC1\x00\x00\x00\x00\xFF\x15\x00\x00\x00\x00\x85\xC0")
    SDK_API /*virtual*/ void removeListener(Listener *l);

    SAPPHIRE_API("1.21.2,1.21.50,1.21.60", "call", "\xE9\x00\x00\x00\x00\xFF\x15\x00\x00\x00\x00\xCC\xCC\xCC\xCC\xC6\x41")
    SDK_API void _fireAppSuspended();
};
#if MC_VERSION == v1_21_2
static_assert(sizeof(AppPlatform) == 1032);
#elif MC_VERSION == v1_21_50 || MC_VERSION == v1_21_60
static_assert(sizeof(AppPlatform) == 1040);
#endif

template <>
SAPPHIRE_API("1.21.2", "call", "\xE8\x00\x00\x00\x00\x90\x48\x8B\x8B\x00\x00\x00\x00\x48\x8B\x01\x48\x8B\x40")
SAPPHIRE_API("1.21.50", "call", "\xE8\x00\x00\x00\x00\x90\xC6\x45\x00\x00\x48\x8B\x8D\x00\x00\x00\x00\x48\x85\xC9\x74\x00\xF0\xFF\x41")
SAPPHIRE_API("1.21.60", "call", "\xE8\x00\x00\x00\x00\x90\x48\x8B\x8D\x00\x00\x00\x00\x48\x8B\x01\x48\x8B\x40\x00\xFF\x15\x00\x00\x00\x00\x48\x8B\xBC\x24")
SDK_API ServiceReference<AppPlatform> ServiceLocator<AppPlatform>::get();
