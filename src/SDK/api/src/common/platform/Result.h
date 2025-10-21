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

        bool m_has_value;

        constexpr Result() :
            m_has_value(true) {}

        constexpr Result(const Result &other) :
            m_has_value(other.m_has_value) {
            if (m_has_value)
                new (&m_value) value_type(other.m_value);
            else
                new (&m_error) error_type(other.m_error);
        }

        constexpr Result(Result &&other) :
            m_has_value(other.m_has_value) {
            if (m_has_value)
                new (&m_value) value_type(std::move(other.m_value));
            else
                new (&m_error) error_type(std::move(other.m_error));
        };

        template <std::convertible_to<error_type> U>
        constexpr Result(U &&val) :
            m_has_value(false) {
            new (&m_error) error_type(std::forward<U>(val));
        }

        template <std::convertible_to<value_type> U>
        constexpr Result(U &&val) :
            m_has_value(true) {
            new (&m_value) value_type(std::forward<U>(val));
        }

        constexpr ~Result() {
            if (m_has_value)
                m_error.~error_type();
            else
                m_value.~value_type();
        }
    };

    // size: 72 (Result<void, std::error_code>)
    template <typename E>
    class Result<void, E> {
    public:
        using value_type = void;
        using error_type = Bedrock::ErrorInfo<E>;

        union {
            char       m_dummy;
            error_type m_error;
        };

        bool m_has_value;

        constexpr Result() :
            m_has_value(true) {}

        constexpr Result(const Result &other) :
            m_has_value(other.m_has_value) {
            if (!m_has_value)
                new (&m_error) error_type(other.m_error);
        }

        constexpr Result(Result &&other) :
            m_has_value(other.m_has_value) {
            if (!m_has_value)
                new (&m_error) error_type(std::move(other.m_error));
        };

        constexpr ~Result() {
            if (!m_has_value)
                m_error.~error_type();
        }
    };
    static_assert(sizeof(Result<void>) == 72);

} // namespace Bedrock