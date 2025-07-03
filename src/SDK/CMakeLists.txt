file(GLOB_RECURSE SDK_SRC api/*.cpp)
file(GLOB_RECURSE CORE_SRC core/*.cpp)

add_library(sapphire_core SHARED ${CORE_SRC} ${SDK_SRC})

add_definitions(-DDLLEXPORT)

target_link_libraries(sapphire_core PUBLIC SapphireCommon minhook imgui kiero d3d11)
