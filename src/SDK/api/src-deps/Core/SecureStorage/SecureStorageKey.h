#pragma once

#include <string>

// size: 40
class SecureStorageKey {
public:
    std::string key;       // off+0
    bool        isEncoded; // off+32
};

// size: 8
class ISecureStorageKeySystem {
public:
    // vtb+0
    virtual ~ISecureStorageKeySystem();

    // vtb+1
    virtual SecureStorageKey getSecureStorageKey(const std::string &key) = 0;

    // vtb+2
    virtual void setSecureStorageKey(const std::string &key, const SecureStorageKey &value) = 0;
};