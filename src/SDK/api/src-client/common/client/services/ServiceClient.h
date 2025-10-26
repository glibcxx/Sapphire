#pragma once

#include "SDK/api/src-deps/Core/Utility/EnableNonOwnerReferences.h"
#include "SDK/api/src-deps/Core/Utility/ServiceLocator.h"

class TaskGroup;
class IMinecraftEventing;
namespace Social {
    class IUserManager;
}

// size: 288 (1.21.50)
class ServiceClient : public Bedrock::EnableNonOwnerReferences {
public:
    std::mutex                             mUnk24;             // off+24
    std::vector<std::shared_ptr<void>>     mSubmittedRequests; // off+104
    std::vector<std::shared_ptr<void>>     mSentRequests;      // off+128
    std::unique_ptr<TaskGroup>             mTaskGroup;         // off+152
    ServiceReference<IMinecraftEventing>   mEventing;          // off+160
    ServiceReference<Social::IUserManager> mUserManager;       // off+200
    std::chrono::steady_clock::time_point  mStartTime;         // off+240
    bool                                   mHasTriedSigningIn; // off+248
    bool                                   mHasStoppedSignIn;  // off+249
    std::string                            mUnk256;            // off+256
};