add_library(render
    renderer.h
    utils.h
    texture_target.h
    texture_target.cpp)

add_subdirectory(cpu)

target_link_libraries(render PUBLIC
    Qt6::OpenGL
    OpenGL::GL)

if(ENABLE_CUDA)
    add_subdirectory(cuda)

    target_compile_definitions(render PUBLIC HAVE_CUDA=1)
endif()
