file(GLOB_RECURSE imgui_src ./*.cpp)

add_library(imgui STATIC ${imgui_src})

target_link_libraries(imgui PUBLIC d3d12.lib dxgi.lib)
