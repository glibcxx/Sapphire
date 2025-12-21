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
