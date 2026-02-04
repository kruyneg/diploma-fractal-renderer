#include "app/ui/fractal_window.h"

#include <QApplication>
#include <QDockWidget>

#include "app/fractal_app.h"
#include "app/ui/input_controller.h"
#include "app/ui/renderer_widget.h"
#include "app/ui/settings_widget.h"

namespace ui {

FractalWindow::FractalWindow(FractalApp* app) : app_(app) {
  if (!app) {
    throw std::invalid_argument("FractalWindow: app is nullptr");
  }

  renderer_widget_ = new RendererWidget(this, app_->renderer());
  connect(renderer_widget_, &RendererWidget::ViewResized, this,
          [this](uint32_t w, uint32_t h) { app_->settings().Resize(w, h); });

  settings_widget_ = new SettingsWidget(this, &app->settings());
  settings_dock_ = new QDockWidget("Settings", this);
  settings_dock_->setWidget(settings_widget_);
  settings_dock_->setAllowedAreas(Qt::LeftDockWidgetArea |
                                  Qt::RightDockWidgetArea);
  settings_dock_->setFloating(false);

  connect(renderer_widget_, &RendererWidget::FrameStatsUpdated,
          settings_widget_, &SettingsWidget::SetFrameStats);

  app_->settings().AddObserver([this] { renderer_widget_->update(); });
  app_->settings().AddObserver(
      [this] { settings_widget_->SyncWithSettings(); });

  frame_timer_.start();
  update_timer_.setInterval(16);
  connect(&update_timer_, &QTimer::timeout, this, [this] {
    const double dt = frame_timer_.restart() * 0.001;
    renderer_widget_->UpdateSettings(dt);
    app_->settings().Commit();
  });

  update_timer_.start();

  setCentralWidget(renderer_widget_);
  addDockWidget(Qt::RightDockWidgetArea, settings_dock_);
  resize(640, 480);
}

FractalApp* FractalWindow::app() { return app_; }

}  // namespace ui
