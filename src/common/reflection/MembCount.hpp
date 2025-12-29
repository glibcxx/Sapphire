#pragma once

#include "Concept.hpp"
#include <cstddef>
#include <type_traits>
#include <utility>

namespace sapphire::refl {

    namespace detial {

        struct Any {
            std::size_t c;

            template <typename T>
                requires(!std::is_copy_constructible_v<T> && !std::is_move_constructible_v<T>)
            constexpr operator T() const;

            template <typename T>
                requires std::is_copy_constructible_v<T>
            constexpr operator T &() const;

            template <typename T>
                requires std::is_move_constructible_v<T>
            constexpr operator T &&() const;

            template <typename T>
                requires std::is_array_v<T>
            constexpr operator T &() const;

            template <typename T>
                requires std::is_array_v<T>
            constexpr operator T &&() const;
        };

        template <typename T, bool Last = false>
        constexpr std::size_t countR(auto &&...args) {
            constexpr bool constructible = requires { T{args...}; };
            if constexpr (!constructible && Last)
                return sizeof...(args) - 1;
            else if constexpr (sizeof...(args) < 32)
                return countR<T, constructible>(args..., detial::Any{});
            else
                static_assert(false, "Too members, max: 256");
        }

        template <typename T, std::size_t Total, std::size_t... Before, std::size_t... Array, std::size_t... After>
        constexpr std::size_t testArrayAt(
            std::index_sequence<Before...>,
            std::index_sequence<Array...>,
            std::index_sequence<After...>,
            std::size_t &nextPos,
            std::size_t  count = 0
        ) {
            constexpr bool canBeArray = requires {
                T{
                    detial::Any{Before}..., {detial::Any{Array}...}, detial::Any{After}...
                };
            };
            constexpr bool canBeStruct = requires {
                T{
                    detial::Any{Before}..., {detial::Any{Array}...}, detial::Any{}, detial::Any{After}...
                };
            };
            if (sizeof...(Before) != nextPos)
                return 0;
            if constexpr (canBeArray && !canBeStruct) {
                nextPos = sizeof...(Before) + sizeof...(Array) - 1;
                return count + sizeof...(Array) - 1;
            } else if constexpr (sizeof...(After) > 0) {
                return testArrayAt<T, Total>(
                    std::make_index_sequence<sizeof...(Before)>{},
                    std::make_index_sequence<sizeof...(Array) + 1>{},
                    std::make_index_sequence<Total - 1 - sizeof...(Before) - sizeof...(Array)>{},
                    nextPos,
                    count
                );
            } else {
                ++nextPos;
                return count;
            }
        }

        template <typename T, std::size_t Total, std::size_t Pos>
        constexpr std::size_t testArrayAt(std::size_t &nextPos) {
            return testArrayAt<T, Total>(
                std::make_index_sequence<Pos>{},
                std::make_index_sequence<2>{},
                std::make_index_sequence<Total - 2 - Pos>{},
                nextPos
            );
        }

        template <typename T, std::size_t Total>
            requires(Total > 1)
        constexpr std::size_t countArray() {
            return []<std::size_t... Before>(std::index_sequence<Before...>) {
                std::size_t count = 0;
                std::size_t nextPos = 0;
                ((count += testArrayAt<T, Total, Before>(nextPos)), ...);
                return count;
            }(std::make_index_sequence<Total - 1>{});
        }

        template <typename T>
        constexpr std::size_t count() {
            constexpr std::size_t count = countR<T>();
            if constexpr (count > 1)
                return count - countArray<T, count>();
            else
                return count;
        }

    } // namespace detial

    template <typename T>
    constexpr std::size_t membCount = []() {
        static_assert(false, "T must be aggregate");
    };

    template <MembCountable T>
    constexpr std::size_t membCount<T> = detial::count<std::remove_cvref_t<T>>();

} // namespace sapphire::refl