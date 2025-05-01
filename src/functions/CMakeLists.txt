file(GLOB_RECURSE
    tickrate_src tickrate/*.cpp
    gui_src guioverlay/*.cpp
    smoothpiston_src smoothpiston/*.cpp
)

add_library(builtin_plugin SHARED
    ./builtinPlugin.cpp
    ${tickrate_src}
    ${gui_src}
    ${smoothpiston_src}
)

target_link_libraries(builtin_plugin PUBLIC sapphire_core kiero fmod_vc)
