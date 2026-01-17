#pragma once

#include <cstdint>
#include <string>

typedef uint64_t HashType64;

// size: 48
class HashedString {
public:
    constexpr HashedString() :
        mStrHash(0) {}

    constexpr HashedString(std::nullptr_t) :
        mStrHash(0) {}

    constexpr HashedString(HashType64 hash, const char *str) :
        mStrHash(hash),
        mStr(str) {}

    constexpr HashedString(HashType64 hash, const std::string &str) :
        mStrHash(hash),
        mStr(str) {}

    constexpr HashedString(const std::string &str) :
        mStrHash(HashedString::computeHash(str)),
        mStr(str) {}

    constexpr HashedString(const char *str) :
        mStrHash(HashedString::computeHash(str)),
        mStr(str) {}

    static constexpr HashType64 computeHash(const char *str) {
        return computeHash(std::string{str});
    }

    static constexpr HashType64 computeHash(const std::string &str) {
        HashType64 hash = 0xCBF29CE484222325;
        for (char s : str) {
            hash = s ^ (0x100000001B3ull * hash);
        }
        return hash;
    }

    constexpr bool operator==(HashType64 rhs) const {
        return this->mStrHash == rhs;
    }

    constexpr bool operator!=(HashType64 rhs) const {
        return this->mStrHash != rhs;
    }

    constexpr bool operator==(const std::string &rhs) const {
        return this->mStr == rhs;
    }

    constexpr bool operator!=(const std::string &rhs) const {
        return this->mStr != rhs;
    }

    HashType64  mStrHash; // off+0
    std::string mStr;     // off+8

    const HashedString *mLastMatch = nullptr; // off+40
};

static_assert(sizeof(HashedString) == 48);