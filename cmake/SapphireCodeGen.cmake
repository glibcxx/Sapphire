function(sapphire_run_codegen SAPPHIRE_CODEGEN_EXE CURRENT_VERSION)

    set(GEN_DIR "${CMAKE_BINARY_DIR}/generated")

    set(OUT_DEF "${GEN_DIR}/bedrock_def+mc${CURRENT_VERSION}.def")
    set(OUT_BIN "${GEN_DIR}/bedrock_sigs+mc${CURRENT_VERSION}.sig.db")

    file(GLOB_RECURSE SDK_HEADERS "${CMAKE_CURRENT_SOURCE_DIR}/src/SDK/api/*.h" "${CMAKE_CURRENT_SOURCE_DIR}/src/SDK/api/*.hpp")

    string(REPLACE "." "_" MC_VERSION_MACRO_FORMAT ${CURRENT_VERSION})

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
        "-mc-versions" "v${MC_VERSION_MACRO_FORMAT}"

        COMMENT "[CodeGen] Scanning headers and generating def/sigs for ${CURRENT_VERSION}..."
        DEPENDS ${SDK_HEADERS} ${SAPPHIRE_CODEGEN_EXE}
        VERBATIM
    )

    set(OUT_LIB "${GEN_DIR}/bedrock_sdk+mc${CURRENT_VERSION}.lib")

    if(MSVC)
        find_program(LIB_EXE NAMES "lib.exe" HINTS ENV "PATH")
    endif()

    if(LIB_EXE)
        # Use MSVC's lib.exe to create the import library
        add_custom_command(
            OUTPUT ${OUT_LIB}
            COMMAND "${LIB_EXE}"
            /def:${OUT_DEF}
            /out:${OUT_LIB}
            /machine:x64
            COMMENT "[Linker] Generating import library ${OUT_LIB} with MSVC lib.exe..."
            DEPENDS ${OUT_DEF}
            VERBATIM
        )
    else()
        # Fallback to llvm-dlltool if lib.exe is not found
        find_program(DLLTOOL_PATH NAMES "llvm-dlltool" "llvm-dlltool.exe" HINTS "${LLVM_TOOLS_BINARY_DIR}")
        if(NOT DLLTOOL_PATH)
            message(FATAL_ERROR "Neither MSVC lib.exe nor llvm-dlltool found! Cannot generate import library.")
        endif()

        add_custom_command(
            OUTPUT ${OUT_LIB}
            COMMAND "${DLLTOOL_PATH}"
            -m i386:x86-64
            -d "${OUT_DEF}"
            -l "${OUT_LIB}"
            COMMENT "[Linker] Generating import library ${OUT_LIB} with llvm-dlltool..."
            DEPENDS ${OUT_DEF}
            VERBATIM
        )
    endif()

    add_custom_target(sapphire_codegen_bedrock_api+mc${CURRENT_VERSION} ALL
        DEPENDS ${OUT_LIB}
    )

    add_library(bedrock_sdk+mc${CURRENT_VERSION} UNKNOWN IMPORTED)
    set_target_properties(bedrock_sdk+mc${CURRENT_VERSION} PROPERTIES
        IMPORTED_LOCATION "${OUT_LIB}"
    )

    install(FILES "${OUT_LIB}"
        DESTINATION ${CMAKE_INSTALL_LIBDIR} COMPONENT sapphire_SDK_lib
    )

    add_custom_target(sapphire_sig_database+mc${CURRENT_VERSION} ALL
        DEPENDS ${OUT_BIN}
    )

    install(FILES "${OUT_BIN}"
        DESTINATION Sapphire/bin/ COMPONENT sapphire_sig_database+mc${CURRENT_VERSION}
    )

    sapphire_add_install_target(
        install-sapphire_sig_database+mc${CURRENT_VERSION}
        COMPONENT sapphire_sig_database+mc${CURRENT_VERSION}
        DEPENDS sapphire_sig_database+mc${CURRENT_VERSION}
    )

endfunction()

function(sapphire_run_codegen_gen_headers SAPPHIRE_CODEGEN_EXE)
    set(GEN_DIR "${CMAKE_BINARY_DIR}/generated")
    set(OUT_HEADERS "${GEN_DIR}/SDK")


    file(GLOB_RECURSE SDK_HEADERS "${CMAKE_CURRENT_SOURCE_DIR}/src/SDK/api/*.h" "${CMAKE_CURRENT_SOURCE_DIR}/src/SDK/api/*.hpp")

    add_custom_command(
        OUTPUT ${OUT_HEADERS}

        COMMAND "${SAPPHIRE_CODEGEN_EXE}"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/SDK/api/src"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/SDK/api/src-client"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/SDK/api/src-deps"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/SDK/api/src-external"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/SDK/api/src-vanilla"
        "-o" "${GEN_DIR}"
        "-p" "${CMAKE_BINARY_DIR}"
        "-mc-versions" "v1_14_514"
        "-gen-headers"

        COMMENT "[CodeGen] Scanning headers and generating prettified headers..."
        DEPENDS ${SDK_HEADERS} ${SAPPHIRE_CODEGEN_EXE}
        VERBATIM
    )

    add_custom_target(sapphire_codegen_gen_header ALL
        DEPENDS ${OUT_HEADERS}
    )

    list(APPEND sapphire_SDK_headers_depends sapphire_codegen_gen_header)
    set(sapphire_SDK_headers_depends ${sapphire_SDK_headers_depends} PARENT_SCOPE)

    install(DIRECTORY "${OUT_HEADERS}/"
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/SDK COMPONENT sapphire_SDK_headers
    )

endfunction()

if(POLICY CMP0169)
    cmake_policy(SET CMP0169 OLD)
endif()

find_program(SAPPHIRE_CODEGEN_EXE NAMES "SapphireCodeGen" "SapphireCodeGen.exe"
    HINTS "${CMAKE_SOURCE_DIR}/tools/bin" "${CMAKE_BINARY_DIR}/tools/bin"
    DOC "Path to the SapphireCodeGen executable"
)

if(NOT SAPPHIRE_CODEGEN_EXE)

    FetchContent_Declare(
        sapphire_codegen
        URL https://github.com/glibcxx/SapphireCodeGen/releases/download/v1.0.0/SapphireCodeGen-1.0.0.zip
        URL_HASH SHA256=3e5789ae05692db7a4ab874034d1b31f4b1377cb7aa9ee96b57c7f8be45babc7
        DOWNLOAD_EXTRACT_TIMESTAMP TRUE
        TLS_VERIFY OFF # it's safe for hash verification is enabled
    )

    FetchContent_GetProperties(sapphire_codegen)
    if(NOT sapphire_codegen_POPULATED)
        message(STATUS "Downloading SapphireCodeGen...")
        FetchContent_Populate(sapphire_codegen)
    endif()
    find_program(SAPPHIRE_CODEGEN_EXE NAMES "SapphireCodeGen" "SapphireCodeGen.exe"
        HINTS "${sapphire_codegen_SOURCE_DIR}/bin"
        DOC "Path to the SapphireCodeGen executable"
    )
endif()

if(BUILD_FOR_ALL_MC_VERSIONS)
    foreach(PROCESSING_MC_VERSION ${SAPPHIRE_SUPPORTED_MC_VERSIONS})
        sapphire_run_codegen(${SAPPHIRE_CODEGEN_EXE} ${PROCESSING_MC_VERSION})
    endforeach()
else()
    sapphire_run_codegen(${SAPPHIRE_CODEGEN_EXE} ${MC_VERSION})
endif()

sapphire_run_codegen_gen_headers(${SAPPHIRE_CODEGEN_EXE})

