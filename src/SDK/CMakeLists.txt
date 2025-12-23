file(GLOB_RECURSE SDK_SRC api/*.cpp)
file(GLOB_RECURSE CORE_SRC core/*.cpp)

macro(sapphire_add_sdk_target BUILDING_MC_VERSION OUT_TARGETS OUT_INSTALL_TARGETS)
    set(SAPPHIRE_TARGET_NAME sapphire_core+mc${BUILDING_MC_VERSION})
    set(SAPPHIRE_LIB_NAME sapphire_core_lib+mc${BUILDING_MC_VERSION})

    add_library(${SAPPHIRE_TARGET_NAME} SHARED)
    target_sources(${SAPPHIRE_TARGET_NAME} PRIVATE ${CORE_SRC} ${SDK_SRC})

    sapphire_enable_release_pdb(${SAPPHIRE_TARGET_NAME})

    target_compile_definitions(${SAPPHIRE_TARGET_NAME} PRIVATE SPHR_DLLEXPORT)

    target_precompile_headers(${SAPPHIRE_TARGET_NAME} PUBLIC
        "${CMAKE_CURRENT_SOURCE_DIR}/../pch.h"
    )

    target_link_libraries(${SAPPHIRE_TARGET_NAME}
        PUBLIC
        SapphireCommon
        bedrock_sdk+mc${BUILDING_MC_VERSION}
        PRIVATE
        minhook imgui kiero d3d11 sapphire_bootloader
    )

    string(REPLACE "." "_" MC_VERSION_MACRO_FORMAT ${BUILDING_MC_VERSION})

    target_compile_definitions(${SAPPHIRE_TARGET_NAME} PRIVATE "MC_VERSION=v${MC_VERSION_MACRO_FORMAT}")

    install(TARGETS ${SAPPHIRE_TARGET_NAME}
        EXPORT SapphireTargets
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR} COMPONENT sapphire_SDK_lib
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR} COMPONENT sapphire_SDK_lib
    )

    install(TARGETS ${SAPPHIRE_TARGET_NAME}
        RUNTIME DESTINATION Sapphire/bin/ COMPONENT ${SAPPHIRE_TARGET_NAME}
    )
    install(FILES $<TARGET_PDB_FILE:${SAPPHIRE_TARGET_NAME}>
        DESTINATION Sapphire/bin/ COMPONENT ${SAPPHIRE_TARGET_NAME}
        OPTIONAL
    )

    sapphire_add_install_target(
        install-${SAPPHIRE_TARGET_NAME}
        COMPONENT ${SAPPHIRE_TARGET_NAME}
    )
    list(APPEND ${OUT_TARGETS} ${SAPPHIRE_TARGET_NAME})
    list(APPEND ${OUT_INSTALL_TARGETS} install-${SAPPHIRE_TARGET_NAME})

endmacro()

set(SAPPHIRE_CORE_TARGETS)
set(INSTALL_SAPPHIRE_CORE_TARGETS)

if(BUILD_FOR_ALL_MC_VERSIONS)
    foreach(BUILDING_MC_VERSION ${SAPPHIRE_SUPPORTED_MC_VERSIONS})
        sapphire_add_sdk_target(${BUILDING_MC_VERSION} SAPPHIRE_CORE_TARGETS INSTALL_SAPPHIRE_CORE_TARGETS)
    endforeach()
else()
    sapphire_add_sdk_target(${MC_VERSION} SAPPHIRE_CORE_TARGETS INSTALL_SAPPHIRE_CORE_TARGETS)
endif()

add_custom_target(sapphire_core DEPENDS ${SAPPHIRE_CORE_TARGETS})
sapphire_add_install_target(
    install-sapphire_core
    DEPENDS ${INSTALL_SAPPHIRE_CORE_TARGETS}
)


# --- Install Headers ---

install(DIRECTORY api/sapphire/
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/SDK/api/sapphire COMPONENT sapphire_SDK_headers
    FILES_MATCHING PATTERN "*.h" PATTERN "*.hpp"
)
install(DIRECTORY core/
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/SDK/core COMPONENT sapphire_SDK_headers
    FILES_MATCHING PATTERN "*.h" PATTERN "*.hpp"
)
