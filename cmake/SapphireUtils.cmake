include(CMakeParseArguments)

function(sapphire_add_install_target target_name)
    cmake_parse_arguments(
        ARG
        ""
        "COMPONENT"
        "DEPENDS"
        ${ARGN}
    )
    if(DEFINED ARG_COMPONENT)
        set(ALL_DEPENDENCIES ${ARG_DEPENDS})
        if(TARGET ${ARG_COMPONENT})
            list(APPEND ALL_DEPENDENCIES ${ARG_COMPONENT})
        endif()

        add_custom_target(${target_name}
            COMMAND ${CMAKE_COMMAND} --install ${CMAKE_BINARY_DIR} --config ${CMAKE_CFG_INTDIR} --component ${ARG_COMPONENT}
            DEPENDS ${ALL_DEPENDENCIES}
        )
    elseif(DEFINED ARG_DEPENDS)
        add_custom_target(${target_name}
            DEPENDS ${ARG_DEPENDS}
        )
    else()
        message(FATAL_ERROR "Neither COMPONENT nor DEPENDS specified for target: ${target_name}")
    endif()
endfunction()

function(sapphire_enable_release_pdb)
    foreach(target_name IN LISTS ARGN)
        if(TARGET ${target_name})
            target_compile_options(${target_name} PRIVATE
                "$<$<CONFIG:Release>:/Zi>"
            )
            target_link_options(${target_name} PRIVATE
                "$<$<CONFIG:Release>:/DEBUG;/OPT:REF;/OPT:ICF>"
            )
        else()
            message(AUTHOR_WARNING "sapphire_enable_release_pdb: Target '${target_name}' does not exist.")
        endif()
    endforeach()
endfunction()


function(sapphire_package_mod target_name)
    cmake_parse_arguments(
        ARG
        ""
        "MANIFEST_TEMPLATE;INSTALL_PATH;MAIN_DLL;MC_VERSION_DEP"
        ""
        ${ARGN}
    )

    if(NOT ARG_MANIFEST_TEMPLATE)
        message(FATAL_ERROR "sapphire_package_mod: MANIFEST_TEMPLATE not specified for target ${target_name}")
    endif()
    if(NOT ARG_INSTALL_PATH)
        message(FATAL_ERROR "sapphire_package_mod: INSTALL_PATH not specified for target ${target_name}")
    endif()
    if(NOT ARG_MAIN_DLL)
        message(FATAL_ERROR "sapphire_package_mod: MAIN_DLL not specified for target ${target_name}")
    endif()
    if(NOT ARG_MC_VERSION_DEP)
        message(FATAL_ERROR "sapphire_package_mod: MC_VERSION_DEP not specified for target ${target_name}")
    endif()

    set(MOD_MANIFEST_OUT "${CMAKE_CURRENT_BINARY_DIR}/manifest.${target_name}.json")

    set(MANIFEST_MAIN_DLL ${ARG_MAIN_DLL})
    set(MANIFEST_MC_VERSION_DEP ${ARG_MC_VERSION_DEP})

    configure_file(
        "${ARG_MANIFEST_TEMPLATE}"
        "${MOD_MANIFEST_OUT}"
        @ONLY
    )

    set(INSTALL_DEST "${ARG_INSTALL_PATH}")

    install(TARGETS ${target_name}
        RUNTIME DESTINATION ${INSTALL_DEST} COMPONENT ${target_name}
    )
    install(FILES $<TARGET_PDB_FILE:${target_name}>
        DESTINATION ${INSTALL_DEST} COMPONENT ${target_name} OPTIONAL
    )

    install(FILES "${MOD_MANIFEST_OUT}"
        DESTINATION ${INSTALL_DEST}
        RENAME "manifest.json"
        COMPONENT ${target_name}
    )

    sapphire_add_install_target(install-${target_name} COMPONENT ${target_name})

endfunction()

