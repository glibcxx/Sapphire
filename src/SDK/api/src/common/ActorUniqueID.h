#pragma once

#include "SDK/api/src-deps/Core/Math/Math.h"

// size: 8
class ActorRuntimeID {
public:
    uint64_t rawID = 0; // off+0

    constexpr ActorRuntimeID() = default;

    constexpr ActorRuntimeID(uint64_t id) :
        rawID(id) {}

    size_t getHash() const {
        return mce::Math::hash2(this->rawID >> 32, this->rawID & 0xFFFFFFFF);
    }

    constexpr operator bool() const { return this->rawID != 0; }

    constexpr operator uint64_t() const { return this->rawID; }

    constexpr bool operator==(const ActorRuntimeID &rhs) const { return this->rawID == rhs.rawID; }

    constexpr bool operator!=(const ActorRuntimeID &rhs) const { return this->rawID != rhs.rawID; }

    constexpr ActorRuntimeID &operator++() {
        ++this->rawID;
        return *this;
    }
};

// size: 8
struct ActorUniqueID {
    int64_t rawID = -1; // off+0

    constexpr ActorUniqueID() = default;

    constexpr ActorUniqueID(int64_t id) :
        rawID(id) {}

    size_t getHash() const {
        return mce::Math::hash2(this->rawID >> 32, this->rawID & 0xFFFFFFFF);
    }

    constexpr operator bool() const { return this->isValid(); }

    constexpr operator int64_t() const { return this->rawID; }

    constexpr bool operator==(const ActorUniqueID &rhs) const { return this->rawID == rhs.rawID; }

    constexpr bool operator!=(const ActorUniqueID &rhs) const { return this->rawID != rhs.rawID; }

    constexpr bool operator>(const ActorUniqueID &rhs) const { return this->rawID > rhs.rawID; }

    constexpr bool operator<(const ActorUniqueID &rhs) const { return this->rawID < rhs.rawID; }

    constexpr ActorUniqueID &operator++() {
        ++this->rawID;
        return *this;
    }

    constexpr bool isValid() const { return this->rawID != -1; }
};

template <>
struct std::hash<ActorUniqueID> {
    size_t operator()(const ActorUniqueID &_Keyval) const {
        return _Keyval.getHash();
    }
};

template <>
struct std::hash<ActorRuntimeID> {
    size_t operator()(const ActorRuntimeID &_Keyval) const {
        return _Keyval.getHash();
    }
};