#pragma once

#include "StringLiteral.hpp"

namespace sapphire::abi {

    namespace detail {
        template <auto Ptr>
        constexpr auto wrapperName() {
            return []<decltype(Ptr)>() { return __FUNCDNAME__; }.template operator()<Ptr>();
        }

        constexpr std::string_view findANumber(std::string_view in) {
            // <non-negative integer> ::= A@                  # when Number == 0
            //                        ::= <decimal digit - 1> # when 1 <= Number <= 10
            //                        ::= <hex digit> + @     # when Number >= 10
            //
            // <number>               ::= [?] <non-negative integer>

            if (in.empty())
                return {};

            if (in.starts_with("A@")) // number == 0
                return in.substr(2);

            bool negative = in[0] == '?';
            if (negative && in.size() == 1)
                return {};

            auto nonNegative = negative ? in.substr(1) : in;

            char numChar = nonNegative[0];
            if (numChar >= '0' && numChar <= '9')
                return in.substr(0, negative + 1);

            auto numEnd = nonNegative.find('@') + 1;
            if (numEnd == std::string_view::npos)
                return {};

            return in.substr(0, negative + numEnd);
        }
    } // namespace detail

    template <auto Ptr, bool AllowVirtualThunk = false, bool VirtualThunkAsPtr = false>
        requires std::is_pointer_v<decltype(Ptr)> || std::is_member_function_pointer_v<decltype(Ptr)>
    class Decorator {
        constexpr static auto get() {
            // ??$?R <symbol-1> @<lambda_1>@?1???$wrapperName@ <symbol-2> @detail@util@@YA?A_PXZ@QEBA?A_PXZ

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

            constexpr std::string_view name = detail::wrapperName<Ptr>();

            constexpr auto tpltArgBegin = name.find("?R$") + 3;
            constexpr auto symbolBegin = tpltArgBegin + 1;
            constexpr bool memberFn = std::is_member_function_pointer_v<decltype(Ptr)>;
            constexpr bool isVirtualThunk = name.substr(symbolBegin, 4) == "??_9";
            if constexpr (memberFn && !AllowVirtualThunk && isVirtualThunk)
                static_assert(false, "Decorated name for virtual function is not supported."
                                     " Or get virtual thunk with AllowVirtualThunk=true.");

            constexpr auto tpltArgEnd = name.find("@<lambda_1>@?");
            constexpr auto code = name[tpltArgBegin];
            if constexpr (code == '1') {
                constexpr auto symbolEnd = tpltArgEnd;
                constexpr auto length = symbolEnd - symbolBegin;
                return StringLiteral<length>{name.substr(symbolBegin, length)};
            } else if constexpr (code == 'H' || code == 'I' || code == 'J') {
                if constexpr (!isVirtualThunk) {
                    // Function symbols always end with 'Z', and <number> never contains 'Z'
                    constexpr auto symbolEnd = name.rfind('Z', tpltArgEnd) + 1;
                    constexpr auto length = symbolEnd - symbolBegin;
                    return StringLiteral<length>{name.substr(symbolBegin, length)};
                } else if constexpr (VirtualThunkAsPtr) {
                    constexpr auto symbolEnd = tpltArgEnd;
                    constexpr auto length = symbolEnd - symbolBegin;
                    return StringLiteral<length>{name.substr(symbolBegin, length)};
                } else {
                    // virtual thunk is not mangled as a function name
                    // <virtual-mem-ptr-thunk-name> ::= ??_9 <type-name> $B <offset-in-vftable> A <calling-convention>

                    constexpr auto vtbOffsetStart = name.rfind("@$B", tpltArgEnd) + 3;
                    constexpr auto vtbOffsetLength = detail::findANumber(name.substr(vtbOffsetStart)).size();
                    if constexpr (vtbOffsetLength == 0)
                        static_assert(false, "Unsupported symbol");

                    constexpr auto vtbOffsetEnd = vtbOffsetLength + vtbOffsetStart;
                    constexpr auto length = vtbOffsetEnd - symbolBegin + 2 /*length of A + <calling-convention> == 2*/;
                    return StringLiteral<length>{name.substr(symbolBegin, length)};
                }
            } else {
                static_assert(false, "Unsupported symbol");
            }
        }

    public:
        static constexpr auto value = get();
    };

} // namespace sapphire::abi