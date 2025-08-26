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
