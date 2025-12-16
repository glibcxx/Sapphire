#pragma once

#include <string_view>
#include <type_traits>
#include "DecoratedName.hpp"

namespace util::type_traits {

    template <typename T>
    constexpr bool is_func_ptr = std::is_pointer_v<T> && std::is_function_v<std::remove_pointer_t<T>>
                              || std::is_member_function_pointer_v<T>;

    template <auto ptr>
    constexpr bool is_virtual_thunk = ::util::Decorator<ptr, true, true>::value.view().starts_with("??_9");

} // namespace util::type_traits