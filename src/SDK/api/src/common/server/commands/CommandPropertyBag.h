#pragma once

#include "SDK/api/src-deps/Core/Utility/PropertyBag.h"
#include "SDK/api/src/common/world/phys/Vec3.h"

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