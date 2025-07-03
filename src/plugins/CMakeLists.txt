file(GLOB_RECURSE
    tickrate_src tickrate/*.cpp
    gui_src guioverlay/*.cpp
    smoothpiston_src smoothpiston/*.cpp
    chunkborder_src chunkborder/*.cpp
    freecamera_src freecamera/*.cpp
)

add_library(builtin_plugin SHARED
    ./builtinPlugin.cpp
    ${tickrate_src}
    ${gui_src}
    ${smoothpiston_src}
    ${chunkborder_src}
    ${freecamera_src}
)

target_link_libraries(builtin_plugin PRIVATE sapphire_core fmod)

add_custom_command(
    TARGET builtin_plugin POST_BUILD 

    COMMAND ${CMAKE_COMMAND} -E copy
                "$<TARGET_FILE:builtin_plugin>"
                "$<TARGET_FILE_DIR:builtin_plugin>/sapphire/mods/$<TARGET_FILE_NAME:builtin_plugin>"

    COMMAND ${CMAKE_COMMAND} -E echo "Moved $<TARGET_FILE_NAME:builtin_plugin> to sapphire/mods"
    VERBATIM
)
