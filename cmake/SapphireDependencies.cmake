cmake_policy(SET CMP0169 OLD)

include(FetchContent)

set(SAPPHIRE_DEPS_GLM_VERSION 1.0.1)
set(SAPPHIRE_DEPS_NLOHMANN_JSON_VERSION 3.11.3)
set(SAPPHIRE_DEPS_ENTT_VERSION 3.14.0)
set(SAPPHIRE_DEPS_GSL_VERSION 4.0.0)
set(SAPPHIRE_DEPS_CONCURRENTQUEUE_VERSION 1.0.4)

FetchContent_Declare(
    glm
    GIT_REPOSITORY https://github.com/g-truc/glm.git
    GIT_TAG ${SAPPHIRE_DEPS_GLM_VERSION}
)
FetchContent_Declare(
    nlohmann
    GIT_REPOSITORY https://github.com/nlohmann/json.git
    GIT_TAG v${SAPPHIRE_DEPS_NLOHMANN_JSON_VERSION}
)
FetchContent_Declare(
    entt
    GIT_REPOSITORY https://github.com/skypjack/entt.git
    GIT_TAG v${SAPPHIRE_DEPS_ENTT_VERSION}
)
FetchContent_Declare(
    gsl
    GIT_REPOSITORY https://github.com/microsoft/GSL.git
    GIT_TAG v${SAPPHIRE_DEPS_GSL_VERSION}
)
FetchContent_Declare(
    moodycamel
    GIT_REPOSITORY https://github.com/cameron314/concurrentqueue.git
    GIT_TAG v${SAPPHIRE_DEPS_CONCURRENTQUEUE_VERSION}
)

function(fetch_dependency_if_not_exist PKG_NAME FETCH_NAME VERSION)
    find_package(${PKG_NAME} ${VERSION} QUIET)
    if(NOT ${PKG_NAME}_FOUND)
        message(STATUS "  -> ${PKG_NAME} not found. Fetching from git...")
        FetchContent_GetProperties(${FETCH_NAME})
        if(NOT ${FETCH_NAME}_POPULATED)
            FetchContent_Populate(${FETCH_NAME})
            add_subdirectory(${${FETCH_NAME}_SOURCE_DIR} ${${FETCH_NAME}_BINARY_DIR} EXCLUDE_FROM_ALL)
        endif()
    else()
        message(STATUS "  -> Found local ${PKG_NAME}: ${${PKG_NAME}_DIR}")
    endif()
endfunction()

function(sapphire_resolve_dependencies)
    message(STATUS "Resolving Sapphire SDK dependencies...")
    fetch_dependency_if_not_exist(glm glm ${SAPPHIRE_DEPS_GLM_VERSION})
    fetch_dependency_if_not_exist(nlohmann_json nlohmann ${SAPPHIRE_DEPS_NLOHMANN_JSON_VERSION})
    fetch_dependency_if_not_exist(EnTT entt ${SAPPHIRE_DEPS_ENTT_VERSION})
    fetch_dependency_if_not_exist(Microsoft.GSL gsl ${SAPPHIRE_DEPS_GSL_VERSION})

    if(NOT TARGET moodycamel::concurrentqueue)
        message(STATUS "  -> moodycamel::concurrentqueue not found. Fetching from git...")
        FetchContent_GetProperties(moodycamel)
        if(NOT moodycamel_POPULATED)
            FetchContent_Populate(moodycamel)
            add_subdirectory(${moodycamel_SOURCE_DIR} ${moodycamel_BINARY_DIR} EXCLUDE_FROM_ALL)
        endif()
    else()
        message(STATUS "  -> Found target: moodycamel::concurrentqueue")
    endif()
endfunction()


