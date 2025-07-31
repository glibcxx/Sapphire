#pragma once

#include "SDK/api/src-deps/Core/Utility/NonOwnerPointer.h"
#include "PlatformRuntimeInfo.h"

class PushNotificationMessage;
class ActivationUri;
class PropertyBag;
class IMinecraftGame;

enum class FileStorageDirectory : int {
    External = 1,
    AppData = 2,
    None = 3,
}; // FileStorageDirectory

namespace Bedrock {

    class PlatformRuntimeInfo;
    class PlatformBuildInfo;
    class IIslandManager;
    class AppIsland;
    class ActivationArguments;

    // size: 64 (1.21.2), 72 (1.21.50/1.21.60)
    class CommonPlatform {
    public:
        std::unique_ptr<PlatformRuntimeInfo>     mPlatformRuntimeInfo;         // off+8
        std::unique_ptr<PlatformBuildInfo>       mPlatformBuildInfo;           // off+16
        std::unique_ptr<IIslandManager>          mIslandMgr;                   // off+24
        std::unique_ptr<AppIsland>               mApp;                         // off+32
        Bedrock::NonOwnerPointer<IMinecraftGame> mMinecraftGame;               // off+40
        std::atomic<bool>                        mbQueueRenderParameterSignal; // off+64 (1.21.50/1.21.60), 56 (1.21.2)
        std::atomic<bool>                        mHasInitialized;              // off+65 (1.21.50/1.21.60), 57 (1.21.2)

        CommonPlatform(const CommonPlatform &) = delete;

        // vtb+0
        virtual ~CommonPlatform();

        // vtb+1
        virtual bool updatePlatformInfo() = 0;

        // vtb+2
        virtual bool updatePlatformGraphicsInfo() = 0;

        // vtb+3
        virtual void issueShutdown();

        // vtb+4
        virtual void issueSuspend();

        // vtb+5
        virtual void issueResume();

        // vtb+6
        virtual void issueBack();

        // vtb+7
        virtual void issueLowMemory();

        // vtb+8
        virtual void issueFocusLost();

        // vtb+9
        virtual void issueFocusGained();

        // vtb+10
        virtual void issueInputPaneVisible();

        // vtb+11
        virtual void issueInputPaneHidden();

        // vtb+12
        virtual void issueSuspendWarning();

        // vtb+13
        virtual void issueVisibilityChange(bool);

        // vtb+14
        virtual void issueWindowSizeChange(int, int);

        // vtb+15
        virtual void issueDPIChange(float);

        // vtb+16
        virtual void issueOrientationChange(const DisplayOrientation &);

        // vtb+17
        virtual void feedButtonPress(const int &);

        // vtb+18
        virtual void feedKeyPress(const char);

        // vtb+19
        virtual void setTextboxText(const std::string &);

        // vtb+20
        virtual void setStorageDirectory(FileStorageDirectory, bool, const PropertyBag &);

        // vtb+21
        virtual FileStorageDirectory setInitialStorageDirectory(FileStorageDirectory);

        // vtb+22
        virtual FileStorageDirectory getStorageDirectory() const;

        // vtb+23
        virtual bool _preAppCreation(const Bedrock::ActivationArguments &) = 0;

        // vtb+24
        virtual bool _postAppCreation(const Bedrock::ActivationArguments &) = 0;

        // vtb+25
        virtual void _processActivationArguments(const Bedrock::ActivationArguments &) = 0;

        // vtb+26
        virtual bool _update(bool) = 0;

        // vtb+27
        virtual bool _isShuttingDown() = 0;

        // vtb+28
        virtual bool _isShutdown() = 0;

        // vtb+29
        virtual void pushNotificationReceived_Shim(const PushNotificationMessage &) = 0;

        // vtb+30
        virtual void notifyUriListeners_Shim(const ActivationUri &) = 0;

        // vtb+31
        virtual std::string getDeviceId_Shim() const = 0;
    };
#if MC_VERSION == v1_21_2
    static_assert(sizeof(CommonPlatform) == 64);
#elif MC_VERSION == v1_21_50 || MC_VERSION == v1_21_60
    static_assert(sizeof(CommonPlatform) == 72);
#endif

} // namespace Bedrock
