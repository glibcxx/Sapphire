cmake_minimum_required(VERSION 3.20.0)

project(Sapphire)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /utf-8")

add_compile_definitions($<$<CONFIG:Debug>:_ITERATOR_DEBUG_LEVEL=0>)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

add_definitions(-DUNICODE=1 -D_UNICODE=1 -DNOMINMAX -DMC_VERSION=v1_21_60)

link_directories(libs)

add_subdirectory(external/minhook)

include_directories(
    .
    src
    external
    external/imgui
    external/minhook/include
    external/soundtouch/include
)

add_compile_definitions(-DKIERO_INCLUDE_D3D12=1 -DKIERO_USE_MINHOOK=1)

add_library(kiero STATIC
    external/kiero/kiero.cpp
)

add_subdirectory(external/imgui)
add_subdirectory(src/functions)
add_subdirectory(src/injector)
add_subdirectory(src/SDK)

