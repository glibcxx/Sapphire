#pragma once

#include "Version.h"

#define EXPORT_DLL __declspec(dllexport)
#define IMPORT_DLL __declspec(dllimport)

#ifdef DLLEXPORT
#    define SDK_API EXPORT_DLL
#else
#    define SDK_API IMPORT_DLL
#endif

#ifndef IMGUI_API
#    define IMGUI_API SDK_API
#endif

#ifndef SPHR_FUNCDNAME
#    define SPHR_FUNCDNAME \
        util::StringLiteral { __FUNCDNAME__ }
#endif

#ifndef SPHR_UNINIT
#    define SPHR_UNINIT(...) \
        union {                     \
            struct {                \
                __VA_ARGS__         \
            };                      \
        };
#endif

#if !defined(NDEBUG)
#    define SPHR_DEBUG 1
#endif

#define PROJECT_NAME "Sapphire"
#define HOME_FOLDER_NAME "sapphire"

#define VA_EXPAND(...) __VA_ARGS__

#define CONCAT_(a, b) a##b
#define CONCAT(a, b) CONCAT_(a, b)

#define ASSUME(...) __assume(__VA_ARGS__);

#ifndef ENABLE_SAFE_HOOK
#    define ENABLE_SAFE_HOOK 1
#endif
