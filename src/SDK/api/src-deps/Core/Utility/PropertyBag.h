#pragma once

#include "SDK/api/src-deps/json/Value.h"

// size: 24
class PropertyBag {
public:
    PropertyBag() = default;

    PropertyBag(const Json::Value &jsonValue) :
        mJsonValue(jsonValue) {}

    Json::Value mJsonValue;         // off+0
    int         mChangeVersion = 0; // off+16
};