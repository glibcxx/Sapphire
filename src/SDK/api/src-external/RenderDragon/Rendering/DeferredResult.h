#pragma once

#include "pch.h" // IWYU pragma: keep

namespace dragon::rendering {

    namespace details {

        // size: 1
        struct Empty {};

    } // namespace details

    // size: 16
    template <typename Result>
    class DeferredResult {
    public:
        std::shared_ptr<std::variant<details::Empty, Result>> mImpl; // off+0
    };

} // namespace dragon::rendering
