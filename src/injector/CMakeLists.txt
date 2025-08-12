# add_executable(Win32Injector Win32Injector.cpp)
add_executable(sapphire_launcher UWPinjector.cpp sapphire.rc)

target_link_libraries(sapphire_launcher PRIVATE SapphireCommon)

target_precompile_headers(sapphire_launcher PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/../pch.h"
)