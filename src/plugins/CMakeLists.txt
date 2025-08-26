file(GLOB_RECURSE
    tickrate_src tickrate/*.cpp
    gui_src guioverlay/*.cpp
    smoothpiston_src smoothpiston/*.cpp
    chunkborder_src chunkborder/*.cpp
    freecamera_src freecamera/*.cpp
    BCMv2_src BCM_V2/*.cpp
)

function(sapphire_builtin_plugin_add_target BUILDING_MC_VERSION)
    set(MOD_TARGET_NAME "builtin_plugin_for_${BUILDING_MC_VERSION}")

    add_library(${MOD_TARGET_NAME} SHARED
        ./builtinPlugin.cpp
        ${tickrate_src}
        ${gui_src}
        ${smoothpiston_src}
        ${chunkborder_src}
        ${freecamera_src}
        ${BCMv2_src}
    )

    target_link_libraries(${MOD_TARGET_NAME} PRIVATE sapphire_core_for_${BUILDING_MC_VERSION})

    target_precompile_headers(${MOD_TARGET_NAME} PRIVATE
        "${CMAKE_CURRENT_SOURCE_DIR}/../pch.h"
    )

    target_compile_definitions(${MOD_TARGET_NAME} PRIVATE "MC_VERSION=${BUILDING_MC_VERSION}")

    install(TARGETS ${MOD_TARGET_NAME}
        EXPORT SapphireBuiltinPluginTargets
        DESTINATION Sapphire/sapphire/mods
        COMPONENT ${MOD_TARGET_NAME}
    )

    sapphire_add_install_target(install-${MOD_TARGET_NAME} COMPONENT ${MOD_TARGET_NAME})

endfunction()

if(BUILD_FOR_ALL_MC_VERSIONS)
    foreach(BUILDING_MC_VERSION ${SAPPHIRE_SUPPORTED_MC_VERSIONS})
        sapphire_builtin_plugin_add_target(${BUILDING_MC_VERSION})
    endforeach()
else()
    sapphire_builtin_plugin_add_target(${MC_VERSION})
endif()
