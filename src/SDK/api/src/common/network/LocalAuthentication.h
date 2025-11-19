#pragma once

#include "SDK/api/src/common/certificates/PrivateKeyManager.h"

class Certificate;

// size: 120
class LocalAuthentication {
    PrivateKeyManager            mLocalKeys;             // off+0
    std::unique_ptr<Certificate> mSelfSignedCertificate; // off+80
    std::string                  mSelfSignedId;          // off+88
};