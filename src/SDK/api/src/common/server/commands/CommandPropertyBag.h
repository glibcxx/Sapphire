#pragma once

#include "pch.h" // IWYU pragma: keep

#include "SDK/api/src-deps/Core/Utility/PropertyBag.h"

// size: 32
class CommandPropertyBag : public PropertyBag {
public:
    CommandPropertyBag() :
        PropertyBag() {}

    CommandPropertyBag(const Json::Value &jsonValue) :
        PropertyBag(jsonValue) {}

    // vtb+0
    virtual ~CommandPropertyBag() = default;

    std::unique_ptr<CommandPropertyBag> clone() const {
        return std::make_unique<CommandPropertyBag>(this->mJsonValue);
    }
};