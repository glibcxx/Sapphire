#pragma once

#include <bit>

namespace sapphire {

    template <typename F>
    class ScopeGuard {
        F    func;
        bool active;

    public:
        ScopeGuard(F f) :
            func(std::move(f)), active(true) {}

        template <typename Fs>
        ScopeGuard(Fs f1, F f) :
            func(std::move(f)), active(true) {
            f1();
        }

        ~ScopeGuard() {
            if (active) func();
        }

        void dismiss() { active = false; }
    };

} // namespace util