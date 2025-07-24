#pragma once

#include "macros/Macros.h"

/*
    代码20行，行行黑魔法
*/
namespace util::detail {
    template <auto value>
    struct ADL {
        friend auto hookable_flag(decltype(value), ADL);
    };

    template <auto value>
    struct set_hookable {
        friend auto hookable_flag(decltype(value), ADL<value>) {}
    };

    template <auto ptr>
    concept is_hookable = requires { hookable_flag(ptr, detail::ADL<ptr>{}); };
} // namespace util::detail

#if ENABLE_HOOKABLE_STATIC_ASSERT == 1
#    define MARK_HOOKABLE(...) \
        static_assert(sizeof(::util::detail::set_hookable<__VA_ARGS__>));

#    define MARK_VIRTUAL_HOOKABLE(...) \
        friend void CONCAT(workaround_for_msvc_in_class_member_ptr_for_virtual_function, __LINE__)() { ::util::detail::set_hookable<__VA_ARGS__>{}; }

#    define ASSERT_HOOKABLE(MSG, ...) \
        static_assert(::util::detail::is_hookable<__VA_ARGS__>, MSG);
#else
#    define MARK_HOOKABLE(...)
#    define MARK_VIRTUAL_HOOKABLE(...)
#    define ASSERT_HOOKABLE(...)
#endif