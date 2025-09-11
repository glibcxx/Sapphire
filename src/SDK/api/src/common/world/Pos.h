#pragma once

#include <cstdint>

class Pos {
public:
    Pos() {}

    Pos(int x, int y, int z) :
        x(x), y(y), z(z) {}

    Pos(const Pos &position) :
        x(position.x), y(position.y), z(position.z) {}

    bool operator==(const Pos &rhs) const {
        return this->x == rhs.x && this->y == rhs.y && this->z == rhs.z;
    }

    bool operator!=(const Pos &rhs) const {
        return *this == rhs;
    }

    Pos operator-(const Pos &rhs) const {
        return Pos{this->x - rhs.x, this->y - rhs.y, this->z - rhs.z};
    }

    Pos operator+(int o) const {
        return Pos{this->x + o, this->y + o, this->z + o};
    }

    Pos operator+(const Pos &rhs) const {
        return Pos{this->x + rhs.x, this->y + rhs.y, this->z + rhs.z};
    }

    Pos operator*(int s) const {
        return Pos{this->x * s, this->y * s, this->z * s};
    }

    size_t hashCode() const;

    Pos offset(int, int, int) const;

    void set(int, int, int);

    void set(const Pos &);

    float distSqrToCenter(float, float, float) const;

    int distSqr(int, int, int) const;

    int x = 0; // off+0
    int y = 0; // off+4
    int z = 0; // off+8
};
