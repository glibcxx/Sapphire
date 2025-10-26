#pragma once

#include "SDK/api/src-deps/Core/Utility/EnableNonOwnerReferences.h"
#include "SDK/api/src-deps/Core/Utility/pubsub/Subscription.h"

// size: 32
class ProfilingManager : public Bedrock::EnableNonOwnerReferences {
public:
    // size: 136 (1.21.50)
    class Impl {
    public:
        using ConfigEnabledGroups = std::set<std::string>;
        using MutexType = std::mutex;
        using LockType = std::lock_guard<std::mutex>;

        ConfigEnabledGroups           mConfigEnabledGroups;        // off+0
        bool                          mGroupEnabledDefaultSetting; // off+16
        MutexType                     mGroupsMutex;                // off+24
        Bedrock::PubSub::Subscription mNewGroupSubscription;       // off+104
        size_t                        mMinGroupStackDepth;         // off+120
        bool                          mHeadersEnabled;             // off+128
    };

    std::unique_ptr<Impl> mImpl; // off+24
};