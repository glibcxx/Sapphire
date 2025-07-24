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

#if !defined(NDEBUG)
#    define SPHR_DEBUG 1
#endif

#define PROJECT_NAME "Sapphire"
#define HOME_FOLDER_NAME "sapphire"

#define VA_EXPAND(...) __VA_ARGS__

#define CONCAT_(a, b) a##b
#define CONCAT(a, b) CONCAT_(a, b)

#define ASSUME(...) __assume(__VA_ARGS__);

#ifndef ENABLE_HOOKABLE_STATIC_ASSERT
#    define ENABLE_HOOKABLE_STATIC_ASSERT 1
#endif
