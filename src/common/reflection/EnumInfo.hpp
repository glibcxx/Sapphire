#pragma once

#include <algorithm>
#include <cctype>
#include <optional>
#include <type_traits>
#include <utility>
#include "MembInfo.hpp"
#include "common/StringLiteral.hpp"

namespace sapphire::refl {

    // [min, max]
    struct EnumRange {
        int64_t min;
        int64_t max;

        constexpr std::size_t size() const { return max - min + 1; }
        constexpr bool        valid() const { return max >= min; }
    };

    constexpr EnumRange defualtEnumRange{-128, 127};

    namespace detail {

        constexpr bool isValidEnumName(std::string_view view) {
#if defined(__clang__)
            auto p = view.find_last_of(')');
            return p == std::string_view::npos
                || view[p + 1] == ':'; // (anonymous namespace)::Enum::Val
#elif defined(__GNUC__) || defined(_MSC_VER)
            return view[0] != '(';
#endif
        }

        template <auto E>
        consteval auto enumName() {
#if !defined(__PRETTY_FUNCTION__) && !defined(__GNUC__)
            std::string_view prettyName = __FUNCSIG__;
#else
            std::string_view prettyName = __PRETTY_FUNCTION__;
#endif

#if defined(__clang__) || defined(__GNUC__)
            prettyName = prettyName.substr(0, prettyName.size() - 1).substr(prettyName.find_last_of('=') + 2);
#elif defined(_MSC_VER)
            prettyName = prettyName.substr(0, prettyName.size() - 7).substr(prettyName.find_last_of('<') + 1);
#else
            static_assert(false, "Unsupported compiler.");
#endif
            if (!isValidEnumName(prettyName))
                return prettyName;
            else
                return prettyName.substr(prettyName.find_last_of(':') + 1);
        }

        template <typename T, T E>
        consteval auto enumNameSliced() {
            constexpr auto nameView = enumName<E>();
            return util::StaticString<util::StringLiteral<nameView.size()>{nameView}>::storage.view();
        }

        template <typename E>
        struct EnumValName {
            E                value;
            std::string_view name;
        };

        template <typename E, EnumRange R, typename T, T... Is>
        consteval auto getEnumeratorsImpl() {
            std::array<EnumValName<E>, sizeof...(Is)> results{};
            size_t                                    count = 0;
            ([&] {
                if constexpr (requires { enumNameSliced<E, static_cast<E>(Is + R.min)>();}) {
                    constexpr auto value = static_cast<E>(Is + R.min);
                    constexpr auto name = enumNameSliced<E, value>();
                    if (isValidEnumName(name))
                        results[count++] = {value, name};
                } }(), ...);
            return std::make_pair(results, count);
        }

        template <typename E, EnumRange R, typename T, T... Is>
        consteval auto getEnumerators(std::integer_sequence<T, Is...>) {
            static_assert(R.valid(), "Invalid EnumRange");
            constexpr auto                             results = getEnumeratorsImpl<E, R, T, Is...>();
            std::array<EnumValName<E>, results.second> resultsSliced;
            for (auto idx = 0; idx < results.second; ++idx) {
                resultsSliced[idx] = results.first[idx];
            }
            return resultsSliced;
        }

    } // namespace detail

    template <typename E, EnumRange R = defualtEnumRange>
        requires std::is_enum_v<E>
    constexpr auto enumNames = detail::getEnumerators<E, R>(
        std::make_integer_sequence<std::underlying_type_t<E>, R.size()>()
    );

    template <typename E, EnumRange R = defualtEnumRange>
        requires std::is_enum_v<E>
    constexpr auto enumCount = enumNames<E, R>.size();

    template <typename E, EnumRange R = defualtEnumRange>
        requires std::is_enum_v<E>
    constexpr auto nameToEnum = [] {
        auto arr = enumNames<E, R>;
        std::ranges::sort(arr, {}, &detail::EnumValName<E>::name);
        return arr;
    }();

    struct _fn_EnumName {
        template <typename E, EnumRange R = defualtEnumRange>
            requires std::is_enum_v<E>
        constexpr auto operator()(E value) const {
            auto it = std::ranges::lower_bound(enumNames<E, R>, value, {}, &detail::EnumValName<E>::value);
            if (it != enumNames<E>.end() && it->value == value)
                return it->name;
            else
                return std::string_view{};
        }
    };

    constexpr _fn_EnumName enumName{};

    template <typename E, EnumRange R>
    struct _fn_EnumValue {
        constexpr std::optional<E> operator()(std::string_view name) const {
            auto it = std::ranges::lower_bound(nameToEnum<E, R>, name, {}, &detail::EnumValName<E>::name);
            if (it != nameToEnum<E>.end() && it->name == name)
                return static_cast<E>(it->value);
            else
                return std::nullopt;
        }
    };

    template <typename E, EnumRange R = defualtEnumRange>
        requires std::is_enum_v<E>
    constexpr _fn_EnumValue<E, R> enumValue{};

} // namespace sapphire::refl