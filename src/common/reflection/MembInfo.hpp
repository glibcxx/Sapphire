#pragma once

#include <algorithm>
#include <string_view>
#include <array>
#include <tuple>
#include <type_traits>
#include <utility>
#include "Concept.hpp"
#include "MembCount.hpp"
#include "common/Overloaded.hpp"
#include "common/StringLiteral.hpp"

namespace sapphire::refl {

    namespace detail {

        template <typename T, auto ptr>
        consteval auto memberName() {
#if !defined(__PRETTY_FUNCTION__) && !defined(__GNUC__)
            constexpr std::string_view prettyName = __FUNCSIG__;
#else
            constexpr std::string_view prettyName = __PRETTY_FUNCTION__;
#endif

#if defined(__clang__)
            return prettyName.substr(0, prettyName.size() - 1).substr(prettyName.find_last_of('.') + 1);
#elif defined(__GNUC__)
            return prettyName.substr(0, prettyName.size() - 2).substr(prettyName.find_last_of(':') + 1);
#elif defined(_MSC_VER)
            return prettyName.substr(0, prettyName.size() - 7).substr(prettyName.find_last_of('-') + 2);
#else
            static_assert(false, "Unsupported compiler.");
#endif
        }

        template <typename T, auto ptr>
        consteval auto memberNameSliced() {
            constexpr auto nameView = memberName<T, ptr>();
            return util::StaticString<util::StringLiteral<nameView.size()>{nameView}>::storage.view();
        }

        template <typename T>
        struct DeclVal {
            inline static std::remove_cvref_t<T> value{};
        };

        template <class T>
        T &&declval();

        template <typename T, std::size_t N>
        struct MemberInfoImpl;

#define SAPPHIRE_REFL_DETAIL_MEMBERINFOIMPL(N, ...)                                                     \
    template <typename T>                                                                               \
    struct MemberInfoImpl<T, N> {                                                                       \
        template <std::size_t... Idx>                                                                   \
        static constexpr std::array<std::string_view, N> getNames(std::index_sequence<Idx...>) {        \
            constexpr auto members = expand();                                                          \
            return {memberNameSliced<T, &std::get<Idx>(members)>()...};                                 \
        };                                                                                              \
        template <std::size_t... Idx>                                                                   \
        static constexpr auto getTypes(std::index_sequence<Idx...>) {                                   \
            constexpr auto members = expand();                                                          \
            return declval<std::tuple<std::remove_reference_t<decltype(std::get<Idx>(members))>...>>(); \
        };                                                                                              \
        static constexpr auto expand() {                                                                \
            auto &[__VA_ARGS__] = DeclVal<T>::value;                                                    \
            return std::tie(__VA_ARGS__);                                                               \
        }                                                                                               \
        static constexpr auto expand(T &value) {                                                        \
            auto &[__VA_ARGS__] = value;                                                                \
            return std::tie(__VA_ARGS__);                                                               \
        }                                                                                               \
    }; // namespace detail

#include "MembInfo.inl"

        template <typename T>
        constexpr auto membNames() {
            constexpr std::size_t count = membCount<T>;
            if constexpr (count > 0)
                return MemberInfoImpl<T, count>::getNames(std::make_index_sequence<count>{});
            else
                return std::array<std::string_view, 0>{};
        }

        template <typename T>
        constexpr auto membTypes() {
            constexpr std::size_t count = membCount<T>;
            if constexpr (count > 0)
                return MemberInfoImpl<T, count>::getTypes(std::make_index_sequence<count>{});
            else
                return std::tuple<>{};
        }

    } // namespace detail

    template <typename T>
    constexpr auto membNames = []() {
        static_assert(false, "T must be sapphire::refl::MembReflcable");
    };

    template <MembReflectable T>
    constexpr auto membNames<T> = detail::membNames<std::remove_cvref_t<T>>();

    template <typename T>
    struct TypeStorage {
        static_assert(false, "T must be sapphire::refl::MembReflcable");
    };

    template <MembReflectable T>
    struct TypeStorage<T> {
        using tuple = decltype(detail::membTypes<std::remove_cvref_t<T>>());

        template <std::size_t Idx>
        using getN = std::tuple_element_t<Idx, tuple>;
    };

    template <typename T>
    using membTypes = TypeStorage<T>;

    class _fn_ForEachMember {
    public:
        template <typename T, typename... Fn>
        constexpr void operator()(T &value, Fn &&...callback) const {
            auto memberTuple = detail::MemberInfoImpl<T, membCount<T>>::expand(value);
            [&, overloaded = util::Overloaded{std::forward<Fn>(callback)...}]<std::size_t... Idx>(
                std::index_sequence<Idx...>
            ) {
                (overloaded(std::get<Idx>(memberTuple), Idx, std::get<Idx>(membNames<T>)), ...);
            }(std::make_index_sequence<membCount<T>>{});
        }

        template <typename T, typename... Fn>
        constexpr void operator()(T &&value, Fn &&...callback) const {
            static_assert(false, "Rvalue is not supported!");
        }
    };

    constexpr _fn_ForEachMember forEachMember{};

} // namespace sapphire::refl