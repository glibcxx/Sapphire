file(GLOB_RECURSE
    tickrate_src tickrate/*.cpp
    gui_src guioverlay/*.cpp
    smoothpiston_src smoothpiston/*.cpp
    chunkborder_src chunkborder/*.cpp
)

add_library(builtin_plugin SHARED
    ./builtinPlugin.cpp
    ${tickrate_src}
    ${gui_src}
    ${smoothpiston_src}
    ${chunkborder_src}
)

target_link_libraries(builtin_plugin PRIVATE sapphire_core fmod)
