#pragma once

#include "StringLiteral.hpp"
#include "macros/Macros.h"

// 本文件已弃用

namespace sapphire {

    template <std::size_t N, typename Callback>
    struct [[deprecated]] Signature;

    template <std::size_t N>
    struct [[deprecated]] Signature<N, nullptr_t> {
        StringLiteral<N> mSig;

        SPHR_FORCE_INLINE constexpr uintptr_t operator()(uintptr_t addr) const { return addr; }
    };

    template <std::size_t N, typename Callback>
        requires std::is_invocable_r_v<uintptr_t, Callback, uintptr_t>
    struct [[deprecated]] Signature<N, Callback> {
        StringLiteral<N> mSig;
        Callback         mCallback;

        SPHR_FORCE_INLINE constexpr uintptr_t operator()(uintptr_t addr) const { return mCallback(addr); }
    };

    template <std::size_t N, typename Callback>
        requires std::is_invocable_r_v<uintptr_t, Callback, uintptr_t>
    [[deprecated]] consteval auto operator|(Signature<N, nullptr_t> sig, Callback fn) {
        return Signature<N, Callback>{sig.mSig, fn};
    }

    template <std::size_t N, typename Callback>
        requires std::is_invocable_r_v<uintptr_t, Callback, uintptr_t>
    [[deprecated]] consteval auto operator|(Callback fn, Signature<N, nullptr_t> sig) {
        return Signature<N, Callback>{sig.mSig, fn};
    }

} // namespace sapphire

template <sapphire::StringLiteral A>
[[deprecated]] consteval auto operator""_sig() {
    return sapphire::Signature<A.size, nullptr_t>{A};
}