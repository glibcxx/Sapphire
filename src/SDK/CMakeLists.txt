file(GLOB_RECURSE SDK_SRC api/*.cpp)

add_library(sapphire_core SHARED core/Core.cpp ${SDK_SRC})

add_definitions(-DDLLEXPORT)

target_link_libraries(sapphire_core PUBLIC minhook)
