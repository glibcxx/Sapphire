#pragma once

#include "SDK/core/ApiManager.h"
#include "SDK/api/src-deps/Core/Math/Math.h"

// size: 12
class BlockPos {
public:
    int x, y, z;

    constexpr BlockPos() :
        BlockPos(0) {}
    constexpr BlockPos(int n) :
        x(n), y(n), z(n) {}
    constexpr BlockPos(int _x, int _y, int _z) :
        x(_x), y(_y), z(_z) {}

    constexpr BlockPos operator+(const BlockPos &other) const {
        return BlockPos(x + other.x, y + other.y, z + other.z);
    }

    constexpr BlockPos operator+(int s) const {
        return BlockPos(x + s, y + s, z + s);
    }

    constexpr BlockPos operator-(const BlockPos &other) const {
        return BlockPos(x - other.x, y - other.y, z - other.z);
    }

    constexpr BlockPos operator-(int s) const {
        return BlockPos(x - s, y - s, z - s);
    }

    constexpr BlockPos operator-() const {
        return BlockPos(-x, -y, -z);
    }

    constexpr BlockPos operator*(const BlockPos &other) const {
        return BlockPos(x * other.x, y * other.y, z * other.z);
    }

    constexpr BlockPos &operator+=(const BlockPos &other) {
        x += other.x, y += other.y, z += other.z;
        return *this;
    }

    constexpr BlockPos &operator+=(int s) {
        x += s, y += s, z += s;
        return *this;
    }

    constexpr BlockPos &operator-=(const BlockPos &other) {
        x -= other.x, y -= other.y, z -= other.z;
        return *this;
    }

    constexpr BlockPos &operator-=(int s) {
        x -= s, y -= s, z -= s;
        return *this;
    }

    constexpr BlockPos &operator*=(const BlockPos &other) {
        x *= other.x, y *= other.y, z *= other.z;
        return *this;
    }

    constexpr bool operator==(const BlockPos &other) const {
        return x == other.x && y == other.y && z == other.z;
    }

    constexpr bool operator!=(const BlockPos &other) const {
        return x != other.x || y != other.y || z != other.z;
    }

    std::string toString() const {
        return std::format("[{}, {}, {}]", x, y, z);
    }

    size_t hashCode() const {
        return mce::Math::hash3(this->x, this->y, this->z);
    }
};

template <>
struct std::hash<BlockPos> {
    std::size_t operator()(const BlockPos &_Keyval) const {
        return _Keyval.hashCode();
    }
};