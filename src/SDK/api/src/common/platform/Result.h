#pragma once

#include "ErrorInfo.h"

namespace Bedrock {

    template <typename T, typename E = std::error_code>
    class Result {
    public:
        using value_type = T;
        using error_type = Bedrock::ErrorInfo<E>;

        union {
            value_type m_value;
            error_type m_error;
        };

        bool m_has_value = false;
    };

    // size: 72 (Result\<void, std::error_code \>)
    template <typename E>
    class Result<void, E> {
    public:
        using value_type = void;
        using error_type = Bedrock::ErrorInfo<E>;

        union {
            char       m_dummy;
            error_type m_error;
        };

        bool m_has_value = false;

        ~Result() = delete;
    };
    static_assert(sizeof(Result<void>) == 72);

} // namespace Bedrock