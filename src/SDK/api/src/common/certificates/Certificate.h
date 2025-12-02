#pragma once

#include "WebToken.h"
#include "SDK/api/src/common/CommonTypes.h"

// size: 163
class UnverifiedCertificate {
public:
    const WebToken                mRawToken;                    // off+0
    Unique<UnverifiedCertificate> mParentUnverifiedCertificate; // off+128

    ~UnverifiedCertificate() = default;
};

// size: 152
class Certificate {
public:
    UnverifiedCertificate mUnverifiedCertificate; // off+0
    Unique<Certificate>   mParentCertificate;     // off+136
    bool                  mIsValid;               // off+144
    bool                  mIsSelfSigned;          // off+145

    ~Certificate() = default;
};