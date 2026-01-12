#pragma once

#include <algorithm>
#include <bit>
#include <concepts>
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
            return sapphire::StaticString<sapphire::StringLiteral<nameView.size()>{nameView}>::storage.view();
        }

        template <typename T>
        struct DeclVal {
            inline static std::remove_cvref_t<T> value{};
        };

        template <class T>
        T &&declval();

        template <typename T, std::size_t N>
        struct MemberInfoImpl;

#define SAPPHIRE_REFL_DETAIL_MEMBERINFOIMPL(N, ...)                  \
    template <typename T>                                            \
    struct MemberInfoImpl<T, N> {                                    \
        static constexpr auto expand(T &value = DeclVal<T>::value) { \
            auto &[__VA_ARGS__] = value;                             \
            return std::tie(__VA_ARGS__);                            \
        }                                                            \
    };

#include "MembInfo.inl"

        template <typename T, std::size_t N>
        struct MemberInfo : MemberInfoImpl<T, N> {
            using MemberInfoImpl<T, N>::expand;

            template <std::size_t... Idx>
            static constexpr std::array<std::string_view, N> getNames(std::index_sequence<Idx...>) {
                constexpr auto members = expand();
                return {memberNameSliced<T, &std::get<Idx>(members)>()...};
            }
            template <std::size_t... Idx>
            static constexpr auto getTypes(std::index_sequence<Idx...>) {
                constexpr auto members = expand();
                return declval<std::tuple<std::remove_reference_t<decltype(std::get<Idx>(members))>...>>();
            }
            template <std::size_t... Idx>
            static constexpr std::array<std::size_t, N> offsets(std::index_sequence<Idx...>) {
                constexpr auto members = expand();
                return {
                    std::bit_cast<uintptr_t>(&std::get<Idx>(members))
                    - std::bit_cast<uintptr_t>(&DeclVal<T>::value)...
                };
            }
        };

        template <typename T>
        struct MemberInfo<T, 0> {
            static constexpr std::tuple<> expand(T &value = DeclVal<T>::value) { return {}; }
            template <std::size_t... Idx>
            static constexpr std::array<std::string_view, 0> getNames(std::index_sequence<Idx...>) { return {}; }
            template <std::size_t... Idx>
            static constexpr std::tuple<> getTypes(std::index_sequence<Idx...>) { return {}; };
            template <std::size_t... Idx>
            static constexpr std::array<std::size_t, 0> offsets(std::index_sequence<Idx...>) { return {}; }
        };

        template <typename T>
        constexpr auto membNames() {
            constexpr std::size_t count = membCount<T>;
            return MemberInfo<T, count>::getNames(std::make_index_sequence<count>{});
        }

        template <typename T>
        constexpr auto membTypes() {
            constexpr std::size_t count = membCount<T>;
            return MemberInfo<T, count>::getTypes(std::make_index_sequence<count>{});
        }

        template <typename T>
        constexpr auto membOffsets() {
            constexpr std::size_t count = membCount<T>;
            return MemberInfo<T, count>::offsets(std::make_index_sequence<count>{});
        }

    } // namespace detail

    template <typename T>
    constexpr auto membNames = []() {
        static_assert(false, "T must be sapphire::refl::MembReflcable");
    };

    template <MembReflectable T>
    constexpr auto membNames<T> = detail::membNames<std::remove_cvref_t<T>>();

    template <MembReflectable T>
    inline const auto membOffsets = detail::membOffsets<std::remove_cvref_t<T>>();

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
            auto memberTuple = detail::MemberInfo<T, membCount<T>>::expand(value);
            [&, overloaded = sapphire::Overloaded{std::forward<Fn>(callback)...}]<std::size_t... Idx>(
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

    template <typename T>
    class _fn_ForEachMemberT {
    public:
        template <typename... Fn>
        constexpr void operator()(Fn &&...callback) const {
            [&, overloaded = sapphire::Overloaded{std::forward<Fn>(callback)...}]<std::size_t... Idx>(
                std::index_sequence<Idx...>
            ) {
                ([&]() {
                    if constexpr (std::is_invocable_v<
                                      decltype(overloaded),
                                      std::type_identity<typename membTypes<T>::template getN<Idx>>,
                                      std::size_t,
                                      std::string_view>)
                        overloaded(
                            std::type_identity<typename membTypes<T>::template getN<Idx>>{},
                            Idx,
                            std::get<Idx>(membNames<T>)
                        );
                    else if constexpr (std::is_invocable_v<
                                           decltype(overloaded),
                                           std::type_identity<typename membTypes<T>::template getN<Idx>>,
                                           std::index_sequence<Idx>,
                                           std::string_view>)
                        overloaded(
                            std::type_identity<typename membTypes<T>::template getN<Idx>>{},
                            std::index_sequence<Idx>{},
                            std::get<Idx>(membNames<T>)
                        );
                    else
                        static_assert(false, "Invalid callback");
                }(),
                 ...);
            }(std::make_index_sequence<membCount<T>>{});
        }
    };

    template <typename T>
    constexpr _fn_ForEachMemberT<T> forEachMemberT{};

} // namespace sapphire::refl