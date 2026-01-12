#include "app/ui/fractal_window.h"

#include <QResizeEvent>

#include "app/fractal_app.h"
#include "app/ui/input_controller.h"
#include "app/ui/renderer_widget.h"


namespace ui {

FractalWindow::FractalWindow(FractalApp* app) : app_(app) {
  if (!app) {
    throw std::invalid_argument("FractalWindow: app is nullptr");
  }

  renderer_widget_ = new RendererWidget(this, app_->renderer());
  input_controller_ = new InputController(this, &app_->settings());

  app_->settings().AddObserver([this] { renderer_widget_->update(); });

  setCentralWidget(renderer_widget_);
  resize(640, 480);
}

void FractalWindow::resizeEvent(QResizeEvent* event) {
  app_->settings().Resize(event->size().width(), event->size().width());
}

void FractalWindow::keyPressEvent(QKeyEvent* event) {
  input_controller_->HandleKeyPress(event);
  QMainWindow::keyPressEvent(event);
}

}  // namespace ui
