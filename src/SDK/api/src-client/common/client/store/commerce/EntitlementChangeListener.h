#pragma once

// size: 32
class EntitlementChangeListener : public std::enable_shared_from_this<EntitlementChangeListener> {
public:
    bool mRefreshingEntitlements; // off+24

    // vtb+0
    virtual ~EntitlementChangeListener();

    // vtb+1
    virtual void onEntitlementReset();

    // vtb+2
    virtual void _onEntitlementChanged();
};