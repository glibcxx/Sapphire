#pragma once

#ifdef DLLEXPORT
#    define SDK_API __declspec(dllexport)
#    define SDK_VAPI __declspec(dllexport)
#else
#    define SDK_API __declspec(dllimport)
#    define SDK_VAPI __declspec(dllimport)
#endif

#define VA_EXPAND(...) __VA_ARGS__

#define CONCAT_(a, b) a##b
#define CONCAT(a, b) CONCAT_(a, b)

#define COUNT_ARGS_IMPL(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, N, ...) N
#define COUNT_ARGS(...) VA_EXPAND(COUNT_ARGS_IMPL(__VA_ARGS__, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0))

#define ARG_NAME(n) a##n
#define ARG_DECL_1(t1) t1 ARG_NAME(0)
#define ARG_DECL_2(t1, t2) t1 ARG_NAME(0), t2 ARG_NAME(1)
#define ARG_DECL_3(t1, t2, t3) t1 ARG_NAME(0), t2 ARG_NAME(1), t3 ARG_NAME(2)
#define ARG_DECL_4(t1, t2, t3, t4) t1 ARG_NAME(0), t2 ARG_NAME(1), t3 ARG_NAME(2), t4 ARG_NAME(3)
#define ARG_DECL_5(t1, t2, t3, t4, t5) t1 ARG_NAME(0), t2 ARG_NAME(1), t3 ARG_NAME(2), t4 ARG_NAME(3), t5 ARG_NAME(4)

#define ARG_CALL_1() a0
#define ARG_CALL_2() a0, a1
#define ARG_CALL_3() a0, a1, a2
#define ARG_CALL_4() a0, a1, a2, a3
#define ARG_CALL_5() a0, a1, a2, a3, a4