#pragma once

namespace Crypto::Asymmetric {
    class Asymmetric;
}

// size: 48
class KeyManager {
public:
    std::string                                     mPublicKey; // off+8
    std::unique_ptr<Crypto::Asymmetric::Asymmetric> mInstance;  // off+40

    // vtb+0
    virtual ~KeyManager();

    // vtb+1
    virtual bool isValid() const;
};