#pragma once

template <typename T>
struct NewType {
    using Raw = T;

    Raw value;

    NewType(const Raw &t_) :
        value(t_) {}

    NewType() :
        value() {}

    NewType(const NewType &t_) :
        value(t_.value) {}

    NewType &operator=(const NewType &other) {
        value = other.value;
        return *this;
    }

    operator const T &() const {
        return value;
    }

    operator T &() {
        return value;
    }

    bool operator==(const NewType &rhs) const {
        return value == rhs.value;
    }

    bool operator<(const NewType &rhs) const {
        return value < rhs.value;
    }
};