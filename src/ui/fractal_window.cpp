#include "ui/fractal_window.h"

#include <ui/renderer_widget.h>

namespace ui {

FractalWindow::FractalWindow(Renderer* renderer) {
  renderer_widget_ = new RendererWidget(this, renderer);

  setCentralWidget(renderer_widget_);
  resize(640, 480);
}

}  // namespace ui
