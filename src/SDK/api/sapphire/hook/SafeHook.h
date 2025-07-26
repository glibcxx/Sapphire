#pragma once

#include "macros/Macros.h"

/*
    代码20行，行行黑魔法
*/
namespace sapphire::inline hook::detail {
    template <auto value>
    struct ADL {
        friend auto hookable_flag(ADL);
    };

    template <auto value>
    struct set_hookable {
        friend auto hookable_flag(ADL<value>) {}
    };

    template <auto ptr, auto Seed = [] {}>
    concept is_hookable = requires { hookable_flag(detail::ADL<ptr>{}); };
} // namespace sapphire::inline hook::detail

#if ENABLE_SAFE_HOOK == 1
#    define MARK_HOOKABLE(...) \
        static_assert(sizeof(::sapphire::detail::set_hookable<__VA_ARGS__>));

// 你永远可以相信msvc的与众不同，这个逼在类内作用域取虚函数成员指针的结果是错的
#    define MARK_VIRTUAL_HOOKABLE(...)                                              \
        friend void CONCAT(mark_virtual_hookable_workaround_for_msvc, __LINE__)() { \
            ::sapphire::detail::set_hookable<__VA_ARGS__>{};                        \
        }

#    define ASSERT_HOOKABLE(MSG, ...) \
        static_assert(::sapphire::detail::is_hookable<__VA_ARGS__>, MSG);
#else
#    define MARK_HOOKABLE(...)
#    define MARK_VIRTUAL_HOOKABLE(...)
#    define ASSERT_HOOKABLE(...)
#endif

#if ENABLE_SAFE_HOOK == 1
#    define HAS_OVERLOAD(...) \
        [](auto p) constexpr { return !requires(decltype(p)) { &__VA_ARGS__; }; }(int{})

#    define ASSERT_NO_OVERLOAD(MSG, ...) \
        static_assert(!HAS_OVERLOAD(__VA_ARGS__), MSG);

#    define IS_VALID_OVERLOAD(TYPE, NAME) \
        HAS_OVERLOAD(NAME)                \
        ? requires { TYPE & NAME; }       \
        : [](auto p) constexpr { return requires(decltype(p)) { TYPE &NAME == &NAME; }; }(int{})

#    define ASSERT_VALID_OVERLOAD(MSG, TYPE, NAME) \
        static_assert(IS_VALID_OVERLOAD(TYPE, NAME), MSG);
#else
#    define HAS_OVERLOAD(...)
#    define ASSERT_NO_OVERLOAD(...)
#    define IS_VALID_OVERLOAD(...)
#    define ASSERT_VALID_OVERLOAD(...)
#endif