add_library(render
    renderer.h
    settings_provider.h)

add_subdirectory(cpu)
add_subdirectory(common)

target_link_libraries(render PUBLIC
    Qt6::OpenGL
    OpenGL::GL)

if(ENABLE_CUDA)
    add_subdirectory(cuda)

    target_compile_definitions(render PUBLIC HAVE_CUDA=1)
endif()
