#pragma once

#include <bit>

namespace util {
    template <typename F>
    class ScopeGuard {
        F    func;
        bool active;

    public:
        ScopeGuard(F f) :
            func(std::move(f)), active(true) {}
        ~ScopeGuard() {
            if (active) func();
        }

        void dismiss() { active = false; }
    };

} // namespace util