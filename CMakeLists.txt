cmake_minimum_required(VERSION 3.20.0)

project(Sapphire)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(MSVC)
    set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreadedDLL")
else()
    message(FATAL_ERROR "[ERROR] Currently do not support other compilers. Please use MSVC.")
endif()

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# --- FetchContent 模块 ---
include(FetchContent)

# --- 输出目录 ---
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# --- GLM 数学库 ---
FetchContent_Declare(
    glm
    GIT_REPOSITORY https://github.com/g-truc/glm.git
    GIT_TAG 1.0.1
)

# --- Json库 ---
FetchContent_Declare(
    nlohmann
    GIT_REPOSITORY https://github.com/nlohmann/json.git
    GIT_TAG v3.12.0
)

# --- EnTT库 ---
FetchContent_Declare(
    entt
    GIT_REPOSITORY https://github.com/skypjack/entt.git
    GIT_TAG v3.14.0
)

# --- GSL库 ---
FetchContent_Declare(
    gsl
    GIT_REPOSITORY https://github.com/microsoft/GSL.git
    GIT_TAG v4.0.0
)

# --- moodycamel::ConcurrentQueue ---
FetchContent_Declare(
    moodycamel
    GIT_REPOSITORY https://github.com/cameron314/concurrentqueue.git
    GIT_TAG v1.0.4
)

# --- 项目版本与兼容性 ---
# 允许从 CMake 命令行设置 MC_VERSION, 例如: cmake -DMC_VERSION=vX_Y_Z .. 
# 默认为 "v1_21_60"
set(MC_VERSION_DEFAULT "v1_21_60")
set(MC_VERSION "${MC_VERSION_DEFAULT}" CACHE STRING "Minecraft compatible version (v1_21_60, v1_21_50, v1_21_2)")
message(STATUS "Configuring Sapphire for MC_VERSION: ${MC_VERSION}")

# --- 启用编译期安全 hook 检测 ---
set(ENABLE_SAFE_HOOK_DEFAULT ON)
set(ENABLE_SAFE_HOOK "${ENABLE_SAFE_HOOK_DEFAULT}"
    CACHE STRING "Enable safe hook. (ON/OFF)")

# --- 全局项目定义 ---
add_compile_definitions(
    UNICODE
    _UNICODE
    NOMINMAX
    "MC_VERSION=${MC_VERSION}"
    "ENABLE_SAFE_HOOK=${ENABLE_SAFE_HOOK}"
)

# --- 外部依赖 ---
add_subdirectory(external/minhook)
add_subdirectory(external/imgui)
FetchContent_MakeAvailable(glm)
FetchContent_MakeAvailable(nlohmann)
FetchContent_MakeAvailable(entt)
FetchContent_MakeAvailable(gsl)
FetchContent_MakeAvailable(moodycamel)

# --- Kiero 库 (没有CMakeLists，手动添加) ---
add_library(kiero STATIC
    external/kiero/kiero.cpp
)
# kiero库的两个参数
target_compile_definitions(kiero PRIVATE
    KIERO_INCLUDE_D3D12=1
    KIERO_USE_MINHOOK=1
)
# kiero 头文件目录
target_include_directories(kiero PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/external/kiero
    ${CMAKE_CURRENT_SOURCE_DIR}/external # for minhook
)

target_link_libraries(kiero PRIVATE minhook)

# --- 公共项目头文件和库 ---
add_library(SapphireCommon INTERFACE)
target_include_directories(SapphireCommon INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${CMAKE_CURRENT_SOURCE_DIR}/external
    ${CMAKE_CURRENT_SOURCE_DIR}/external/imgui
    ${CMAKE_CURRENT_SOURCE_DIR}/external/minhook/include
)
target_link_directories(SapphireCommon INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/libs
)
target_link_libraries(SapphireCommon INTERFACE glm nlohmann_json EnTT::EnTT Microsoft.GSL::GSL concurrentqueue)

target_compile_options(SapphireCommon INTERFACE
    "/utf-8"
)
target_compile_definitions(SapphireCommon INTERFACE
    UNICODE
    _UNICODE
    NOMINMAX
    WIN32_LEAN_AND_MEAN
    "MC_VERSION=${MC_VERSION}"
    ENTT_SPARSE_PAGE=2048
    ENTT_PACKED_PAGE=128
    $<$<CONFIG:Debug>:_ITERATOR_DEBUG_LEVEL=0>
)

target_precompile_headers(SapphireCommon INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src/pch.h>
)

add_subdirectory(src/plugins)
add_subdirectory(src/injector)
add_subdirectory(src/SDK)
