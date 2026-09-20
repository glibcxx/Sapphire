#pragma once

#include "macros/Macros.h"

#define CO_CHECK(...)                                                        \
    do {                                                                     \
        auto _expectedRes = __VA_ARGS__;                                     \
        if (_expectedRes.hasError()) [[unlikely]]                            \
            co_return sapphire::Unexpected{std::move(_expectedRes.error())}; \
    } while (0);

#define CO_ASSIGN_OP_IMPL(lhs, op, cnt, ...)                                                \
    auto SPHR_CONCAT(_expectedRes_, cnt) = __VA_ARGS__;                                     \
    if (SPHR_CONCAT(_expectedRes_, cnt).hasError()) [[unlikely]]                            \
        co_return sapphire::Unexpected{std::move(SPHR_CONCAT(_expectedRes_, cnt).error())}; \
    lhs op std::move(SPHR_CONCAT(_expectedRes_, cnt).value());

#define CO_ASSIGN_OP(lhs, op, ...) CO_ASSIGN_OP_IMPL(lhs, op, __COUNTER__, __VA_ARGS__)
#define CO_ASSIGN(var, ...) CO_ASSIGN_OP(var, =, __VA_ARGS__)

#define CO_TRY  switch (auto _sapphire_co_try_block = [&]()

#define CO_CATCH(e)                                                         \
    ; 0) case 0:                                                            \
    switch (auto _sapphire_co_try_block_task = _sapphire_co_try_block(); 0) \
    case 0:                                                                 \
        if (auto &e = co_await _sapphire_co_try_block_task; e.hasError()) [[unlikely]]

#define CO_CATCH_IF(e, ...)                                                 \
    ; 0) case 0:                                                            \
    switch (auto _sapphire_co_try_block_task = _sapphire_co_try_block(); 0) \
    case 0:                                                                 \
        if (auto &e = co_await _sapphire_co_try_block_task; e.hasError() && __VA_ARGS__) [[unlikely]]

#define X_CHECK(...)                                                      \
    do {                                                                  \
        auto _expectedRes = __VA_ARGS__;                                  \
        if (_expectedRes.hasError()) [[unlikely]]                         \
            return sapphire::Unexpected{std::move(_expectedRes.error())}; \
    } while (0);

#define X_ASSIGN_OP_IMPL(lhs, op, cnt, ...)                                              \
    auto SPHR_CONCAT(_expectedRes_, cnt) = __VA_ARGS__;                                  \
    if (SPHR_CONCAT(_expectedRes_, cnt).hasError()) [[unlikely]]                         \
        return sapphire::Unexpected{std::move(SPHR_CONCAT(_expectedRes_, cnt).error())}; \
    lhs op std::move(SPHR_CONCAT(_expectedRes_, cnt).value());

#define X_ASSIGN_OP(lhs, op, ...) X_ASSIGN_OP_IMPL(lhs, op, __COUNTER__, __VA_ARGS__)
#define X_ASSIGN(var, ...) X_ASSIGN_OP(var, =, __VA_ARGS__)
