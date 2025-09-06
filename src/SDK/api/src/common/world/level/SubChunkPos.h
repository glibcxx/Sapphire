#pragma once

#include "BlockPos.h"

class SubChunkPos {
public:
    int x; // off+0
    int y; // off+4
    int z; // off+8

    constexpr SubChunkPos() :
        SubChunkPos(0) {}
    constexpr SubChunkPos(int n) :
        x(n), y(n), z(n) {}
    constexpr SubChunkPos(int _x, int _y, int _z) :
        x(_x), y(_y), z(_z) {}

    constexpr SubChunkPos operator+(const SubChunkPos &other) const {
        return SubChunkPos(x + other.x, y + other.y, z + other.z);
    }

    constexpr SubChunkPos operator+(int s) const {
        return SubChunkPos(x + s, y + s, z + s);
    }

    constexpr SubChunkPos operator-(const SubChunkPos &other) const {
        return SubChunkPos(x - other.x, y - other.y, z - other.z);
    }

    constexpr SubChunkPos operator-(int s) const {
        return SubChunkPos(x - s, y - s, z - s);
    }

    constexpr SubChunkPos operator-() const {
        return SubChunkPos(-x, -y, -z);
    }

    constexpr SubChunkPos operator*(const SubChunkPos &other) const {
        return SubChunkPos(x * other.x, y * other.y, z * other.z);
    }

    constexpr SubChunkPos &operator+=(const SubChunkPos &other) {
        x += other.x, y += other.y, z += other.z;
        return *this;
    }

    constexpr SubChunkPos &operator+=(int s) {
        x += s, y += s, z += s;
        return *this;
    }

    constexpr SubChunkPos &operator-=(const SubChunkPos &other) {
        x -= other.x, y -= other.y, z -= other.z;
        return *this;
    }

    constexpr SubChunkPos &operator-=(int s) {
        x -= s, y -= s, z -= s;
        return *this;
    }

    constexpr SubChunkPos &operator*=(const SubChunkPos &other) {
        x *= other.x, y *= other.y, z *= other.z;
        return *this;
    }

    constexpr bool operator==(const SubChunkPos &other) const {
        return x == other.x && y == other.y && z == other.z;
    }

    constexpr bool operator!=(const SubChunkPos &other) const {
        return x != other.x || y != other.y || z != other.z;
    }

    std::string toString() const {
        return std::format("[{}, {}, {}]", x, y, z);
    }

    operator BlockPos() const {
        return {this->x, this->y, this->z};
    }

    size_t hashCode() const {
        return mce::Math::hash3(this->x, this->y, this->z);
    }
};

template <>
struct std::hash<SubChunkPos> {
    std::size_t operator()(const SubChunkPos &_Keyval) const {
        return _Keyval.hashCode();
    }
};