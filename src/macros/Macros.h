#pragma once

#include "Version.h"

#if defined(_MSC_VER) || defined(__MINGW32__) || defined(__CYGWIN__)
#    define SPHR_EXPORT __declspec(dllexport)
#    define SPHR_IMPORT __declspec(dllimport)
#elif defined(__GNUC__) || defined(__clang__)
#    define SPHR_EXPORT [[gnu::visibility("default")]]
#    define SPHR_IMPORT [[gnu::visibility("default")]]
#else
#    define SPHR_EXPORT
#    define SPHR_IMPORT
#endif

#ifdef SPHR_DLLEXPORT
#    define SPHR_API SPHR_EXPORT
#else
#    define SPHR_API SPHR_IMPORT
#endif

#ifdef SRBL_DLLEXPORT
#    define SRBL_API SPHR_EXPORT
#else
#    define SRBL_API SPHR_IMPORT
#endif

#define SDK_API SPHR_IMPORT

#if defined(_MSC_VER)
#    define SPHR_NOINLINE [[msvc::noinline]]
#elif defined(__GNUC__) || defined(__clang__)
#    define SPHR_NOINLINE [[gnu::noinline]]
#else
#    define SPHR_NOINLINE
#endif

#if defined(_MSC_VER)
#    define SPHR_FORCE_INLINE [[msvc::forceinline]]
#elif defined(__clang__)
#    define SPHR_FORCE_INLINE [[clang::always_inline]]
#elif defined(__GNUC__)
#    define SPHR_FORCE_INLINE [[gnu::always_inline]]
#else
#    define SPHR_FORCE_INLINE
#endif

#ifndef IMGUI_API
#    define IMGUI_API SPHR_API
#endif

#ifndef SPHR_FUNCDNAME
#    define SPHR_FUNCDNAME \
        util::StringLiteral { __FUNCDNAME__ }
#endif

#ifndef SPHR_UNINIT
#    define SPHR_UNINIT(...) \
        union {              \
            struct {         \
                __VA_ARGS__  \
            };               \
        };
#endif

#define SPHR_LINKER_SYM_ALIAS_IMPORT(FROM_NAME, TO_NAME) \
    comment(linker, "/alternatename:__imp_" FROM_NAME "=__imp_" TO_NAME)

#define SPHR_LINKER_SYM_ALIAS_EXPORT(FROM_NAME, TO_NAME) \
    comment(linker, "/alternatename:" FROM_NAME "=" TO_NAME)

#ifdef SPHR_DLLEXPORT
#    define SPHR_LINKER_SYM_ALIAS SPHR_LINKER_SYM_ALIAS_EXPORT
#else
#    define SPHR_LINKER_SYM_ALIAS SPHR_LINKER_SYM_ALIAS_IMPORT
#endif

#if !defined(NDEBUG)
#    define SPHR_DEBUG 1
#endif

#define SPHR_PROJECT_NAME "Sapphire"
#define SPHR_HOME_FOLDER_NAME "sapphire"

#define SPHR_VA_EXPAND(...) __VA_ARGS__

#define SPHR_CONCAT_(a, b) a##b
#define SPHR_CONCAT(a, b) SPHR_CONCAT_(a, b)

#define SPHR_ASSUME(...) __assume(__VA_ARGS__);

#ifndef SPHR_ENABLE_SAFE_HOOK
#    define SPHR_ENABLE_SAFE_HOOK 1
#endif

#ifdef SAPPHIRE_CODEGEN_PASS
#    define SPHR_DECL_API(...) [[clang::annotate("sapphire::bind", __VA_ARGS__)]]
#else
#    define SPHR_DECL_API(...)
#endif