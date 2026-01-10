add_library(render
    renderer.h
    utils.h
    texture_target.h
    texture_target.cpp)

add_subdirectory(cpu)

target_link_libraries(render PRIVATE
    Qt6::OpenGL
    OpenGL::GL)
