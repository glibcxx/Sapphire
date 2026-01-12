#pragma once

#include <string_view>
#include <type_traits>
#include "DecoratedName.hpp"

namespace sapphire {

    template <typename T>
    constexpr bool isFuncPtr = std::is_pointer_v<T> && std::is_function_v<std::remove_pointer_t<T>>
                            || std::is_member_function_pointer_v<T>;

    template <auto ptr>
    constexpr bool isVirtualThunk = abi::Decorator<ptr, true, true>::value.view().starts_with("??_9");

} // namespace sapphire