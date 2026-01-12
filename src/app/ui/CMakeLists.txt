target_sources(app PRIVATE
    fractal_window.h
    fractal_window.cpp
    renderer_widget.h
    renderer_widget.cpp
    texture_target.h
    texture_target.cpp
    input_controller.h
    input_controller.cpp)

target_link_libraries(app PRIVATE
    Qt6::Widgets
    Qt6::OpenGLWidgets
    render)
