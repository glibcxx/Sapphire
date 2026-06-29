#pragma once

/*
    本文件提供了 windows.h 中的各种类型的等价实现，用来尽量避免在头文件中引入 windows.h
*/

#include <cstdint>

struct HWND__;
struct HDC__;
struct HBRUSH__;
struct HINSTANCE__;

namespace sapphire::sys::win {

    using handle_t = void *;
    using dword_t = unsigned long;
    using long_t = long;
    using bool_t = int;
    using byte_t = unsigned char;
    using word_t = unsigned short;
    using float_t = float;
    using longlong_t = __int64;
    using ulonglong_t = unsigned __int64;

    using large_integer_t = longlong_t;

    using long_ptr_t = intptr_t;
    using ulong_ptr_t = uintptr_t;

    using wparam_t = uintptr_t;
    using lparam_t = intptr_t;
    using lresult_t = intptr_t;

    using hwnd_t = HWND__ *;
    using hmodule_t = HINSTANCE__ *;
    using hdc_t = HDC__ *;
    using hbrush_t = HBRUSH__ *;

} // namespace sapphire::sys::win
