#pragma once

#include <memory>

namespace mce {

    template <typename Resource, typename HandleTracker, typename Description>
    struct ResourceBlockTemplate {
        std::shared_ptr<Description> mDebugInfoBlock;
        HandleTracker                mTrackingBlock;
        std::unique_ptr<Resource>    mResource;
    };

} // namespace mce