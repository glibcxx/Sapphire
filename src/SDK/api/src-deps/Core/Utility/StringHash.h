#pragma once

#include <cstdint>
#include <string>

typedef uint64_t HashType64;

class HashedString {
public:
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

    constexpr bool operator==(HashType64 rhs) {
        return this->mStrHash == rhs;
    }

    constexpr bool operator!=(HashType64 rhs) {
        return this->mStrHash != rhs;
    }

    constexpr bool operator==(const std::string &rhs) {
        return this->mStr == rhs;
    }

    constexpr bool operator!=(const std::string &rhs) {
        return this->mStr != rhs;
    }

private:
    HashType64  mStrHash;
    std::string mStr;

    const HashedString *mLastMatch = nullptr;
};

static_assert(sizeof(HashedString) == 48);