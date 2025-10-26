#pragma once

#include "SDK/api/src-client/common/client/store/commerce/EntitlementChangeListener.h"

// size: 96
class GenericEntitlementChangeListener : public EntitlementChangeListener {
public:
    std::function<void()> mEntitlementChangedCallback; // off+32
};
