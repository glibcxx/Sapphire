#pragma once

#ifdef DLLEXPORT
#    define SDK_API __declspec(dllexport)
#else
#    define SDK_API __declspec(dllimport)
#endif

#define SDK_VAPI SDK_API virtual

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