#pragma once

#include "KeyManager.h"

// size: 80
class PrivateKeyManager : public KeyManager {
public:
    std::string mPrivateKey; // off+48
};
