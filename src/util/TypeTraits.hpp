#pragma once

#include <string_view>
#include <type_traits>

namespace util::type_traits {

    namespace detail {
        template <auto ptr>
        consteval auto is_virtual_thunk() {
            constexpr std::string_view name{__FUNCDNAME__};
            constexpr auto             begin = name.find("@$") + 3;
            return name.substr(begin, 4) == "??_9";
        }
    } // namespace detail

    template <typename T>
    constexpr bool is_func_ptr = std::is_pointer_v<T> && std::is_function_v<std::remove_pointer_t<T>>
                              || std::is_member_function_pointer_v<T>;

    template <auto ptr>
    constexpr bool is_virtual_thunk = detail::is_virtual_thunk<ptr>();

} // namespace util::type_traits