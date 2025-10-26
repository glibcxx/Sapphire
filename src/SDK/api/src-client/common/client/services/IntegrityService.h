#pragma once

#include "ServiceClient.h"

class BrazeSDKManager;

// size: 400 (1.21.50)
class IntegrityService : public ServiceClient {
public:
    BrazeSDKManager           *mBrazeSDKManager; // off+288
    std::shared_ptr<void>      mUnk296;          // off+296
    std::shared_ptr<void>      mUnk312;          // off+312
    std::string                mUnk328;          // off+328
    std::optional<std::string> mUnk360;          // off+360
};