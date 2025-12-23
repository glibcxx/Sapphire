file(GLOB_RECURSE
    tickrate_src tickrate/*.cpp
    gui_src guioverlay/*.cpp
    smoothpiston_src smoothpiston/*.cpp
    chunkborder_src chunkborder/*.cpp
    freecamera_src freecamera/*.cpp
    BCMv2_src BCM_V2/*.cpp
)

function(sapphire_builtin_mod_add_target BUILDING_MC_VERSION)
    set(MOD_TARGET_NAME "builtin_mod+mc${BUILDING_MC_VERSION}")

    add_library(${MOD_TARGET_NAME} SHARED
        ./BuiltinMod.cpp
        ${tickrate_src}
        ${gui_src}
        ${smoothpiston_src}
        ${chunkborder_src}
        ${freecamera_src}
        ${BCMv2_src}
    )

    sapphire_enable_release_pdb(${MOD_TARGET_NAME})

    target_link_libraries(${MOD_TARGET_NAME} PRIVATE sapphire_core+mc${BUILDING_MC_VERSION})

    target_precompile_headers(${MOD_TARGET_NAME} PRIVATE
        "${CMAKE_CURRENT_SOURCE_DIR}/../pch.h"
    )

    string(REPLACE "." "_" MC_VERSION_MACRO_FORMAT ${BUILDING_MC_VERSION})
    target_compile_definitions(${MOD_TARGET_NAME} PRIVATE "MC_VERSION=v${MC_VERSION_MACRO_FORMAT}")

    sapphire_package_mod(${MOD_TARGET_NAME}
        MANIFEST_TEMPLATE "${CMAKE_CURRENT_SOURCE_DIR}/manifest.json.in"
        INSTALL_PATH "Sapphire/sapphire/mods/builtin_mod-${SAPPHIRE_VERSION}+mc${BUILDING_MC_VERSION}"
        MAIN_DLL "${MOD_TARGET_NAME}.dll"
        MC_VERSION_DEP ${BUILDING_MC_VERSION}
    )
endfunction()

if(BUILD_FOR_ALL_MC_VERSIONS)
    foreach(BUILDING_MC_VERSION ${SAPPHIRE_SUPPORTED_MC_VERSIONS})
        sapphire_builtin_mod_add_target(${BUILDING_MC_VERSION})
    endforeach()
else()
    sapphire_builtin_mod_add_target(${MC_VERSION})
endif()
