#pragma once

#include <memory>
#include "macros/Macros.h"

namespace sapphire {

    template <typename T>
    class Singleton {
        static std::unique_ptr<T> sInstance;

    public:
        Singleton(const Singleton &) = delete;
        Singleton &operator=(const Singleton &) = delete;

        template <typename... Args>
        static T &create(Args &&...args) {
            if (!sInstance) {
                sInstance = std::make_unique<T>(std::forward<Args>(args)...);
            }
            return *sInstance;
        }

        static void destroy() noexcept {
            if (!sInstance) return;
            sInstance.reset();
        }

        SDK_API static T &getInstance() noexcept {
            return sInstance;
        }
    };

} // namespace sapphire