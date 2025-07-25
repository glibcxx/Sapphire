#pragma once

#include "macros/Macros.h"

/*
    代码20行，行行黑魔法
*/
namespace util::detail {
    template <auto value>
    struct ADL {
        friend auto hookable_flag(ADL);
    };

    template <auto value>
    struct set_hookable {
        friend auto hookable_flag(ADL<value>) {}
    };

    template <auto ptr>
    concept is_hookable = requires { hookable_flag(detail::ADL<ptr>{}); };
} // namespace util::detail

#if ENABLE_HOOKABLE_STATIC_ASSERT == 1
#    define MARK_HOOKABLE(...) \
        static_assert(sizeof(::util::detail::set_hookable<__VA_ARGS__>));

// 你永远可以相信msvc的与众不同，这个逼在类内作用域取虚函数成员指针的结果是错的
#    define MARK_VIRTUAL_HOOKABLE(...)                                              \
        friend void CONCAT(mark_virtual_hookable_workaround_for_msvc, __LINE__)() { \
            ::util::detail::set_hookable<__VA_ARGS__>{};                            \
        }

#    define ASSERT_HOOKABLE(MSG, ...) \
        static_assert(::util::detail::is_hookable<__VA_ARGS__>, MSG);
#else
#    define MARK_HOOKABLE(...)
#    define MARK_VIRTUAL_HOOKABLE(...)
#    define ASSERT_HOOKABLE(...)
#endif