#pragma once

#include <unordered_map>
#include "SDK/api/src-deps/Core/Utility/AutomaticID.h"

class Dimension;

using DimensionType = AutomaticID<Dimension, int>;

template <>
struct std::hash<DimensionType> {
public:
    std::size_t operator()(const DimensionType &_Keyval) const {
        return _Keyval.value();
    }
};