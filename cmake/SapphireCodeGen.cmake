function(sapphire_run_codegen CURRENT_VERSION)

    set(GEN_DIR "${CMAKE_BINARY_DIR}/generated")

    set(OUT_DEF "${GEN_DIR}/bedrock_def.${CURRENT_VERSION}.def")
    set(OUT_BIN "${GEN_DIR}/bedrock_sigs.${CURRENT_VERSION}.sig.db")

    find_program(SAPPHIRE_CODEGEN_EXE NAMES "SapphireCodeGen" "SapphireCodeGen.exe"
        HINTS "${CMAKE_SOURCE_DIR}/tools" "${CMAKE_BINARY_DIR}/tools"
        DOC "Path to the SapphireCodeGen executable"
    )

    if(NOT SAPPHIRE_CODEGEN_EXE)
        message(FATAL_ERROR "SapphireCodeGen tool not found! Please check PATH or set SAPPHIRE_CODEGEN_EXE.")
    endif()

    file(GLOB_RECURSE SDK_HEADERS "${CMAKE_CURRENT_SOURCE_DIR}/src/SDK/api/*.h" "${CMAKE_CURRENT_SOURCE_DIR}/src/SDK/api/*.hpp")

    add_custom_command(
        OUTPUT ${OUT_DEF} ${OUT_BIN}

        COMMAND "${SAPPHIRE_CODEGEN_EXE}"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/SDK/api/src"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/SDK/api/src-client"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/SDK/api/src-deps"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/SDK/api/src-external"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/SDK/api/src-vanilla"
        "-o" "${GEN_DIR}"
        "-p" "${CMAKE_BINARY_DIR}"
        "-mc-versions" "${CURRENT_VERSION}"
        "-resource-dir" "D:/softwares/env/msys2/clang64/lib/clang/20"

        COMMENT "[CodeGen] Scanning headers and generating def/sigs for ${CURRENT_VERSION}..."
        DEPENDS ${SDK_HEADERS} ${SAPPHIRE_CODEGEN_EXE}
        VERBATIM
    )

    find_program(DLLTOOL_PATH NAMES "llvm-dlltool" "llvm-dlltool.exe" HINTS "${LLVM_TOOLS_BINARY_DIR}")
    if(NOT DLLTOOL_PATH)
        message(FATAL_ERROR "llvm-dlltool not found! Cannot generate import library.")
    endif()

    set(OUT_LIB "${GEN_DIR}/bedrock_sdk.${CURRENT_VERSION}.lib")

    add_custom_command(
        OUTPUT ${OUT_LIB}
        COMMAND "${DLLTOOL_PATH}"
        -m i386:x86-64
        -d "${OUT_DEF}"
        -l "${OUT_LIB}"

        COMMENT "[Linker] Generating import library ${OUT_LIB}..."
        DEPENDS ${OUT_DEF}
        VERBATIM
    )

    add_custom_target(sapphire_codegen_bedrock_api_for_${CURRENT_VERSION} ALL
        DEPENDS ${OUT_LIB}
    )

    add_library(sapphire::bedrock_api_${CURRENT_VERSION} UNKNOWN IMPORTED)
    set_target_properties(sapphire::bedrock_api_${CURRENT_VERSION} PROPERTIES
        IMPORTED_LOCATION "${OUT_LIB}"
    )

    install(FILES "${OUT_LIB}"
        DESTINATION ${CMAKE_INSTALL_LIBDIR}
        COMPONENT sapphire_SDK_lib
    )

    install(FILES "${OUT_BIN}"
        DESTINATION Sapphire/bin/
        COMPONENT sapphire_sig_database
    )

endfunction()

if(BUILD_FOR_ALL_MC_VERSIONS)
    foreach(PROCESSING_MC_VERSION ${SAPPHIRE_SUPPORTED_MC_VERSIONS})
        sapphire_run_codegen(${PROCESSING_MC_VERSION})
    endforeach()
else()
    sapphire_run_codegen(${MC_VERSION})
endif()

