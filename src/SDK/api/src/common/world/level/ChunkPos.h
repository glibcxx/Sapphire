#pragma once

#include "BlockPos.h"

class ChunkPos {
public:
    int x;
    int z;

    constexpr ChunkPos() :
        x(0), z(0) {}
    constexpr ChunkPos(int x, int z) :
        x(x), z(z) {}
    constexpr ChunkPos(const BlockPos &blockPos) :
        x(blockPos.x >> 4), z(blockPos.z >> 4) {}

    constexpr ChunkPos operator+(const ChunkPos &other) const {
        return ChunkPos(x + other.x, z + other.z);
    }

    constexpr ChunkPos operator+(int s) const {
        return ChunkPos(x + s, z + s);
    }

    constexpr ChunkPos operator-(const ChunkPos &other) const {
        return ChunkPos(x - other.x, z - other.z);
    }

    constexpr ChunkPos operator-(int s) const {
        return ChunkPos(x - s, z - s);
    }

    constexpr ChunkPos operator-() const {
        return ChunkPos(-x, -z);
    }

    constexpr ChunkPos &operator+=(const ChunkPos &other) {
        x += other.x, z += other.z;
        return *this;
    }

    constexpr ChunkPos &operator+=(int s) {
        x += s, z += s;
        return *this;
    }

    constexpr ChunkPos &operator-=(const ChunkPos &other) {
        x -= other.x, z -= other.z;
        return *this;
    }

    constexpr ChunkPos &operator-=(int s) {
        x -= s, z -= s;
        return *this;
    }

    constexpr bool operator==(const ChunkPos &other) const {
        return x == other.x && z == other.z;
    }

    constexpr bool operator!=(const ChunkPos &other) const {
        return !(*this == other);
    }

    std::string toString() const {
        return std::format("[{}, {}]", x, z);
    }
};