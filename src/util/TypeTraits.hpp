#pragma once

#include <type_traits>

namespace type_traits {

    template <typename T>
    constexpr bool is_func_ptr = std::is_pointer_v<T> && std::is_function_v<std::remove_pointer_t<T>>
                              || std::is_member_function_pointer_v<T>;

} // namespace type_traits