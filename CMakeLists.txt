cmake_minimum_required(VERSION 3.20.0)

set(SAPPHIRE_VERSION "0.1.1")
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

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreadedDLL")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

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
add_subdirectory(external/imgui)
include(cmake/SapphireDependencies.cmake)
sapphire_resolve_dependencies()

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
    $<BUILD_INTERFACE:SapphireDeps>
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
)

add_subdirectory(src/plugins)
add_subdirectory(src/injector)
add_subdirectory(src/SDK)

# --- Installation Rules ---
install(TARGETS SapphireCommon
    EXPORT SapphireTargets
    COMPONENT CPACK_Development
)

install(DIRECTORY external/imgui/
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/imgui COMPONENT CPACK_Headers
    FILES_MATCHING PATTERN "*.h"
)
install(DIRECTORY external/fmod/
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/fmod COMPONENT CPACK_Headers
)
install(FILES libs/fmod.lib
    DESTINATION ${CMAKE_INSTALL_LIBDIR} COMPONENT CPACK_Development
)

install(DIRECTORY src/macros/
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/macros COMPONENT CPACK_Headers
    FILES_MATCHING PATTERN "*.h" PATTERN "*.hpp"
)
install(DIRECTORY src/util/
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/util COMPONENT CPACK_Headers
    FILES_MATCHING PATTERN "*.h" PATTERN "*.hpp"
)

install(FILES cmake/SapphireDependencies.cmake
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/Sapphire COMPONENT CPACK_Development
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
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/Sapphire COMPONENT CPACK_Development
)

install(EXPORT SapphireTargets
    FILE SapphireTargets.cmake
    NAMESPACE Sapphire::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/Sapphire COMPONENT CPACK_Development
)

################################################################################
# CPack Packaging Configuration
################################################################################

set(CPACK_PACKAGE_VERSION ${SAPPHIRE_VERSION})
set(CPACK_PACKAGE_VENDOR "Triteleiopsis")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Sapphire Modding SDK and Runtime")
set(CPACK_PACKAGE_INSTALL_DIRECTORY "Sapphire")

set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/LICENSE")

set(CPACK_GENERATOR "ZIP")
set(CPACK_PACKAGE_FILE_NAME "Sapphire-${SAPPHIRE_VERSION}-win64")

# --- 打包SDK ---

cpack_add_component_group(SDK
    DISPLAY_NAME "Developer SDK"
    DESCRIPTION "Sapphire Development Kit.") # 简称SDK，乐

cpack_add_component(CPACK_Development
    DISPLAY_NAME "Libraries (.lib) and configs (.cmake etc)"
    GROUP SDK)

cpack_add_component(CPACK_Headers
    DISPLAY_NAME "Header Files (.h, .hpp)"
    GROUP SDK)

cpack_add_component(Devel
    DISPLAY_NAME "none"
    DESCRIPTION "from concurrentqueue"
    GROUP SDK HIDDEN)

cpack_add_component(Unspecified
    DISPLAY_NAME "none"
    DESCRIPTION "from 3rd party"
    GROUP SDK HIDDEN)

# --- 打包exe ---

macro(sapphire_pack_sapphire_exe BUILDING_MC_VERSION)
    set(GROUP_NAME mc-${BUILDING_MC_VERSION})
    cpack_add_component_group(${GROUP_NAME}
        DISPLAY_NAME "Sapphire for ${BUILDING_MC_VERSION}"
        DESCRIPTION "Just Sapphire."
        DEPENDS SapphireCore)

    cpack_add_component(CPACK_SapphireLauncher_for_${BUILDING_MC_VERSION}
        DISPLAY_NAME "Sapphire Launcher"
        DESCRIPTION "The executable to launch the game with Sapphire."
        GROUP ${GROUP_NAME})

    cpack_add_component(CPACK_Runtime_for_${BUILDING_MC_VERSION}
        DISPLAY_NAME "Core Runtime (.dll)"
        GROUP ${GROUP_NAME})

    cpack_add_component(CPACK_SapphireBuiltinPlugin_for_${BUILDING_MC_VERSION}
        DISPLAY_NAME "Built-in Plugin"
        GROUP ${GROUP_NAME})

    install(TARGETS sapphire_launcher
        DESTINATION Sapphire/
        COMPONENT CPACK_SapphireLauncher_for_${BUILDING_MC_VERSION}
    )

endmacro()

if(BUILD_FOR_ALL_MC_VERSIONS)
    foreach(BUILDING_MC_VERSION ${SAPPHIRE_SUPPORTED_MC_VERSIONS})
        sapphire_pack_sapphire_exe(${BUILDING_MC_VERSION})
    endforeach()
else()
    sapphire_pack_sapphire_exe(${MC_VERSION})
endif()

set(CPACK_ARCHIVE_COMPONENT_INSTALL TRUE)

include(CPack)
