#pragma once

#include "StringLiteral.hpp"
#include "macros/Macros.h"

namespace sapphire::signature {

    template <std::size_t N, typename Callback>
    struct Signature;

    template <std::size_t N>
    struct Signature<N, nullptr_t> {
        util::StringLiteral<N> mSig;

        SPHR_FORCE_INLINE constexpr uintptr_t operator()(uintptr_t addr) const { return addr; }
    };

    template <std::size_t N, typename Callback>
        requires std::is_invocable_r_v<uintptr_t, Callback, uintptr_t>
    struct Signature<N, Callback> {
        util::StringLiteral<N> mSig;
        Callback               mCallback;

        SPHR_FORCE_INLINE constexpr uintptr_t operator()(uintptr_t addr) const { return mCallback(addr); }
    };

    template <std::size_t N, typename Callback>
        requires std::is_invocable_r_v<uintptr_t, Callback, uintptr_t>
    consteval auto operator|(Signature<N, nullptr_t> sig, Callback fn) {
        return Signature<N, Callback>{sig.mSig, fn};
    }

    template <std::size_t N, typename Callback>
        requires std::is_invocable_r_v<uintptr_t, Callback, uintptr_t>
    consteval auto operator|(Callback fn, Signature<N, nullptr_t> sig) {
        return Signature<N, Callback>{sig.mSig, fn};
    }

} // namespace sapphire::signature

template <util::StringLiteral A>
consteval auto operator""_sig() {
    return sapphire::signature::Signature<A.size, nullptr_t>{A};
}