file(GLOB_RECURSE SDK_SRC api/*.cpp)
file(GLOB_RECURSE CORE_SRC core/*.cpp)

function(sapphire_add_sdk_target BUILDING_MC_VERSION)

    set(SAPPHIRE_TARGET_NAME sapphire_core_for_${BUILDING_MC_VERSION})

    add_library(${SAPPHIRE_TARGET_NAME} SHARED)
    target_sources(${SAPPHIRE_TARGET_NAME} PRIVATE ${CORE_SRC} ${SDK_SRC})

    target_compile_definitions(${SAPPHIRE_TARGET_NAME} PRIVATE DLLEXPORT)

    target_precompile_headers(${SAPPHIRE_TARGET_NAME} PRIVATE
        "${CMAKE_CURRENT_SOURCE_DIR}/../pch.h"
    )

    target_link_libraries(${SAPPHIRE_TARGET_NAME}
        PUBLIC
        SapphireCommon
        PRIVATE
        minhook imgui kiero d3d11
    )

    target_compile_definitions(${SAPPHIRE_TARGET_NAME} PRIVATE "MC_VERSION=${BUILDING_MC_VERSION}")

    install(TARGETS ${SAPPHIRE_TARGET_NAME}
        EXPORT SapphireTargets
        RUNTIME DESTINATION Sapphire/ COMPONENT CPACK_Runtime_for_${BUILDING_MC_VERSION}
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR} COMPONENT CPACK_Development
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR} COMPONENT CPACK_Development
    )

endfunction()

if(BUILD_FOR_ALL_MC_VERSIONS)
    foreach(BUILDING_MC_VERSION ${SAPPHIRE_SUPPORTED_MC_VERSIONS})
        sapphire_add_sdk_target(${BUILDING_MC_VERSION})
    endforeach()
else()
    sapphire_add_sdk_target(${MC_VERSION})
endif()

# --- Install Headers ---

install(DIRECTORY api/
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/SDK/api COMPONENT CPACK_Headers
    FILES_MATCHING PATTERN "*.h" PATTERN "*.hpp"
)
install(DIRECTORY core/
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/SDK/core COMPONENT CPACK_Headers
    FILES_MATCHING PATTERN "*.h" PATTERN "*.hpp"
)
