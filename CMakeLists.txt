cmake_minimum_required(VERSION 3.20.0)

include(cmake/SapphireVersion.cmake)

set(SAPPHIRE_VERSION ${SAPPHIRE_VERSION_MAJOR}.${SAPPHIRE_VERSION_MINOR}.${SAPPHIRE_VERSION_PATCH})
project(Sapphire VERSION ${SAPPHIRE_VERSION})

if(NOT MSVC)
    message(FATAL_ERROR "[ERROR] This project requires the MSVC compiler.")
endif()

if(NOT CMAKE_SIZEOF_VOID_P EQUAL 8)
    message(FATAL_ERROR "[ERROR] This project must be compiled for a 64-bit (x64) architecture.")
endif()

include(FetchContent)
include(CMakePackageConfigHelpers)
include(GNUInstallDirs)
include(cmake/SapphireUtils.cmake)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreadedDLL")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(sapphire_SDK_headers_depends "")

# --- 项目版本与兼容性 ---
include(cmake/SapphireConfigMCVersion.cmake)

# --- 输出目录 ---
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# --- 启用编译期安全 hook 检测 ---
set(ENABLE_SAFE_HOOK_DEFAULT ON)
set(ENABLE_SAFE_HOOK "${ENABLE_SAFE_HOOK_DEFAULT}" CACHE STRING "Enable safe hook. (ON/OFF)")

# --- 外部依赖 ---
add_subdirectory(external/minhook EXCLUDE_FROM_ALL)
add_subdirectory(external/imgui EXCLUDE_FROM_ALL)
include(cmake/SapphireDependencies.cmake)
sapphire_resolve_dependencies()
include(cmake/SapphireCodeGen.cmake)

# --- Dependency Grouping ---
add_library(SapphireDeps INTERFACE)
target_link_libraries(SapphireDeps INTERFACE
    glm::glm
    nlohmann_json::nlohmann_json
    Microsoft.GSL::GSL
    EnTT::EnTT
    concurrentqueue
)

# --- Kiero 库 ---
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
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/external>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/external/imgui>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)
target_link_directories(SapphireCommon INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/libs>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_LIBDIR}>
)
target_link_libraries(SapphireCommon INTERFACE
    fmod
    WindowsApp
    delayimp
    $<BUILD_INTERFACE:SapphireDeps>
)
target_link_options(SapphireCommon INTERFACE
    "/DELAYLOAD:sapphire_bootloader.dll"
)

target_compile_options(SapphireCommon INTERFACE
    "/utf-8"
)
target_compile_definitions(SapphireCommon INTERFACE
    UNICODE
    _UNICODE
    NOMINMAX
    WIN32_LEAN_AND_MEAN
    "ENABLE_SAFE_HOOK=${ENABLE_SAFE_HOOK}"
    ENTT_SPARSE_PAGE=2048
    ENTT_PACKED_PAGE=128
    $<$<CONFIG:Debug>:_ITERATOR_DEBUG_LEVEL=0>

    "SAPPHIRE_VERSION_MAJOR=${SAPPHIRE_VERSION_MAJOR}"
    "SAPPHIRE_VERSION_MINOR=${SAPPHIRE_VERSION_MINOR}"
    "SAPPHIRE_VERSION_PATCH=${SAPPHIRE_VERSION_PATCH}"
    "SAPPHIRE_VERSION=${SAPPHIRE_VERSION}"
    "SAPPHIRE_VERSION_STRING=\"${SAPPHIRE_VERSION}\""
)

add_subdirectory(src/builtinmod)
add_subdirectory(src/injector)
add_subdirectory(src/bootloader)
add_subdirectory(src/SDK)

add_subdirectory(src/test)

# --- Installation Rules ---
install(TARGETS SapphireCommon
    EXPORT SapphireTargets
    COMPONENT CPACK_Development
)

install(DIRECTORY external/imgui/
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR} COMPONENT sapphire_SDK_headers
    FILES_MATCHING PATTERN "*.h"
)
install(DIRECTORY external/fmod/
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/fmod COMPONENT sapphire_SDK_headers
)
install(FILES libs/fmod.lib
    DESTINATION ${CMAKE_INSTALL_LIBDIR} COMPONENT sapphire_SDK_lib
)

install(DIRECTORY src/macros/
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/macros COMPONENT sapphire_SDK_headers
    FILES_MATCHING PATTERN "*.h" PATTERN "*.hpp"
)
install(DIRECTORY src/util/
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/util COMPONENT sapphire_SDK_headers
    FILES_MATCHING PATTERN "*.h" PATTERN "*.hpp"
)
install(FILES src/pch.h
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR} COMPONENT sapphire_SDK_headers
)

install(FILES cmake/SapphireDependencies.cmake
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/Sapphire COMPONENT sapphire_SDK_resource
)

install(FILES cmake/SapphireUtils.cmake
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/Sapphire COMPONENT sapphire_SDK_resource
)

write_basic_package_version_file(
    "${CMAKE_CURRENT_BINARY_DIR}/SapphireConfigVersion.cmake"
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY AnyNewerVersion
)

configure_package_config_file(
    "cmake/SapphireConfig.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/SapphireConfig.cmake"
    INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/Sapphire
)

install(FILES
    "${CMAKE_CURRENT_BINARY_DIR}/SapphireConfig.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/SapphireConfigVersion.cmake"
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/Sapphire COMPONENT sapphire_SDK_resource
)

install(EXPORT SapphireTargets
    FILE SapphireTargets.cmake
    NAMESPACE Sapphire::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/Sapphire COMPONENT sapphire_SDK_resource
)

sapphire_add_install_target(
    install-sapphire_SDK_resource
    COMPONENT sapphire_SDK_resource
)

sapphire_add_install_target(
    install-sapphire_SDK_lib
    COMPONENT sapphire_SDK_lib
    DEPENDS sapphire_core
)

sapphire_add_install_target(
    install-sapphire_SDK_headers
    COMPONENT sapphire_SDK_headers
    DEPENDS ${sapphire_SDK_headers_depends}
)

sapphire_add_install_target(
    install-sapphire_SDK
    DEPENDS install-sapphire_SDK_headers install-sapphire_SDK_resource install-sapphire_SDK_lib
)
