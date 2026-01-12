#pragma once

#include "common/StringLiteral.hpp"

namespace sapphire::refl {

    namespace detail {

        template <typename T>
        consteval auto typeName() {
#if !defined(__PRETTY_FUNCTION__) && !defined(__GNUC__)
            constexpr std::string_view prettyName = __FUNCSIG__;
#else
            constexpr std::string_view prettyName = __PRETTY_FUNCTION__;
#endif
#if defined(__clang__) || defined(__GNUC__)
            return prettyName.substr(0, prettyName.size() - 1).substr(prettyName.find_last_of('=') + 2);
#elif defined(_MSC_VER)
            return prettyName.substr(0, prettyName.size() - 7).substr(prettyName.find_first_of('<') + 1);
#else
            static_assert(false, "Unsupported compiler.");
#endif
        }

        consteval auto removeTypeKeyword(std::string_view name) {
            if (name.starts_with("class ") || name.starts_with("union "))
                return name.substr(6);
            else if (name.starts_with("struct "))
                return name.substr(7);
            else if (name.starts_with("enum "))
                return name.substr(5);
            else
                return name;
        }

        template <typename T, bool removeKeyword>
        consteval auto typeNameSliced() {
            constexpr auto nameView = removeKeyword ? removeTypeKeyword(typeName<T>()) : typeName<T>();
            return sapphire::StaticString<sapphire::StringLiteral<nameView.size()>{nameView}>::storage.view();
        }

    } // namespace detail

    template <typename T, bool removeKeyword = true>
    constexpr auto typeName = detail::typeNameSliced<T, removeKeyword>();

} // namespace sapphire::refl