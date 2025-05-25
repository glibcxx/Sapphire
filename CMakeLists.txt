cmake_minimum_required(VERSION 3.20.0)

project(Sapphire)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(MSVC)
    add_compile_options("/utf-8")
else()
    message(FATAL_ERROR "[ERROR] Currently do not support other compilers. Please use MSVC.")
endif()

# Debug 模式下的迭代器调试级别
# 主要针对 MSVC STL std::string 在不同模式下内存布局不同
add_compile_definitions($<$<CONFIG:Debug>:_ITERATOR_DEBUG_LEVEL=0>)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# --- 输出目录 ---
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib) # 静态库

# --- 项目版本与兼容性 ---
# 允许从 CMake 命令行设置 MC_VERSION, 例如: cmake -DMC_VERSION=vX_Y_Z .. 
# 默认为 "v1_21_60"
set(MC_VERSION_DEFAULT "v1_21_60")
set(MC_VERSION "${MC_VERSION_DEFAULT}" CACHE STRING "Minecraft compatible version (v1_21_60, v1_21_50, v1_21_2)")
message(STATUS "Configuring Sapphire for MC_VERSION: ${MC_VERSION}")

# --- 全局项目定义 ---
add_compile_definitions(
    UNICODE
    _UNICODE
    NOMINMAX
    "MC_VERSION=${MC_VERSION}"
)

# --- 外部依赖 ---
add_subdirectory(external/minhook)
add_subdirectory(external/imgui)

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

add_subdirectory(src/plugins)
add_subdirectory(src/injector)
add_subdirectory(src/SDK)
