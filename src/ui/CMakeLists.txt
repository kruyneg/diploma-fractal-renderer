qt_add_library(ui STATIC
    fractal_window.h
    fractal_window.cpp
    renderer_widget.h
    renderer_widget.cpp)

target_link_libraries(ui PUBLIC
    Qt6::Widgets
    Qt6::OpenGLWidgets
    render)
