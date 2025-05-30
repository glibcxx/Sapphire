#pragma once

template <typename NAMESPACE, typename ID_TYPE>
class AutomaticID {
public:
    typedef AutomaticID SELF;

    AutomaticID(ID_TYPE id = 0) :
        runtimeID(id) {}

    AutomaticID(const SELF &) = default;

    SELF &operator=(const SELF &) = default;

    bool operator==(const SELF &rhs) const {
        return runtimeID == rhs.runtimeID;
    }

    bool operator!=(const SELF &rhs) const {
        return runtimeID != rhs.runtimeID;
    }

    const ID_TYPE &value() const {
        return runtimeID;
    }

private:
    ID_TYPE runtimeID;
};
