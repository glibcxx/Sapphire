#pragma once

#include "StringLiteral.hpp"

namespace util {

    namespace detail {
        template <auto Ptr>
        constexpr auto warpperName() {
            return []<decltype(Ptr)>() { return __FUNCDNAME__; }.template operator()<Ptr>();
        }
    } // namespace detail

    template <auto Ptr, bool AllowVirtualThunk = false>
        requires std::is_pointer_v<decltype(Ptr)> || std::is_member_function_pointer_v<decltype(Ptr)>
    class Decorator {
        constexpr static auto get() {
            // ??$?R <symbol-1> @<lambda_1>@?1???$warpperName@ <symbol-2> @detail@util@@YA?A_PXZ@QEBA?A_PXZ

            // According to llvm:

            // <symbol-1> ::= <func-ptr>
            //            ::= <var-ptr>
            //            ::= <member-function-pointer>

            // <func-ptr> ::= $1? <mangled-name>
            // <var-ptr>  ::= $1? <mangled-name>

            // <member-function-pointer> ::= $1? <name>
            //                           ::= $H? <name> <number>
            //                           ::= $I? <name> <number> <number>
            //                           ::= $J? <name> <number> <number> <number>

            // Function symbols always end with 'Z', and <number> never contains 'Z'

            constexpr std::string_view name = detail::warpperName<Ptr>();

            constexpr auto tpltArgBegin = name.find("?R$") + 3;
            constexpr auto symbolBegin = tpltArgBegin + 1;
            constexpr bool memberFn = std::is_member_function_pointer_v<decltype(Ptr)>;
            if constexpr (memberFn && !AllowVirtualThunk && name.substr(symbolBegin, 4) == "??_9")
                static_assert(false, "Decorated name for virtual function is not supported."
                                     " Or get virtual thunk with AllowVirtualThunk=true.");

            constexpr auto tpltArgEnd = name.find("@<lambda_1>@?");
            constexpr auto code = name[tpltArgBegin];
            if constexpr (code == '1') {
                constexpr auto symbolEnd = tpltArgEnd;
                constexpr auto length = symbolEnd - symbolBegin;
                return StringLiteral<length>{name.substr(symbolBegin, length)};
            } else if constexpr (code == 'H' || code == 'I' || code == 'J') {
                constexpr auto symbolEnd = name.rfind('Z', tpltArgEnd) + 1;
                constexpr auto length = symbolEnd - symbolBegin;
                return StringLiteral<length>{name.substr(symbolBegin, length)};
            } else {
                static_assert(false, "Unsupported symbol");
            }
        }

    public:
        static constexpr auto value = get();
    };

} // namespace util