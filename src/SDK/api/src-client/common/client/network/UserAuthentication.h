#pragma once

#include "SDK/api/src-deps/Core/Threading/TaskGroup.h"
#include "SDK/api/src/common/network/LocalAuthentication.h"
#include "SDK/api/src-deps/Core/Utility/pubsub/Subscription.h"
#include "macros/Macros.h"

class IClientInstance;
class Options;

// size: 520 (1.21.2/1.21.60), 536 (1.21.50)
class UserAuthentication
#if MC_VERSION == v1_21_2 || MC_VERSION == v1_21_50
    : public std::enable_shared_from_this<UserAuthentication>
#endif
{
public:
    class UserListObserver;

    LocalAuthentication                                   mLocalAuthentication;        // off+16
    std::weak_ptr<IClientInstance>                        mClient;                     // off+136
    std::unique_ptr<UserAuthentication::UserListObserver> mUserListObserver;           // off+152
    uint64_t                                              mClientRandomId;             // off+160
    std::shared_ptr<Options>                              mOptions;                    // off+168
    Bedrock::PubSub::Subscription                         mUsernameOptionSubscription; // off+184
    TaskGroup                                             mTaskGroup;                  // off+200

    SDK_API UserAuthentication(std::weak_ptr<IClientInstance> client, uint64_t clientRandomId, const std::string &a3);
#pragma SPHR_LINKER_SYM_ALIAS(                                                                                                                   \
    "??0UserAuthentication@@QEAA@V?$weak_ptr@VIClientInstance@@@std@@_KAEBV?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@2@@Z",        \
    "?ctor@UserAuthentication@@QEAAPEAV1@V?$weak_ptr@VIClientInstance@@@std@@_KAEBV?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@3@@Z" \
)

    SPHR_DECL_API("1.21.2,1.21.50", "\x48\x8B\xC4\x48\x89\x58\x00\x48\x89\x50\x00\x48\x89\x48\x00\x55\x56\x57\x41\x56")
    SPHR_DECL_API("1.21.60", "\x48\x89\x54\x24\x00\x48\x89\x4C\x24\x00\x53\x55\x56\x57\x41\x56\x48\x83\xEC\x00\x49\x8B\xD9\x49\x8B\xE8")
    SDK_API UserAuthentication *ctor(
        std::weak_ptr<IClientInstance> client, uint64_t clientRandomId, const std::string &a3
    );
};