add_library(render_cuda
    utils.h
    utils.cu
    cuda_renderer.h
    cuda_renderer.cu)

set_target_properties(render_cuda PROPERTIES
    CUDA_SEPARABLE_COMPILATION ON)

target_link_libraries(render_cuda
    PUBLIC
        CUDA::cudart
        render)

target_compile_options(render_cuda PRIVATE
    $<$<COMPILE_LANGUAGE:CUDA>:-Xcompiler=/Zc:__cplusplus>)