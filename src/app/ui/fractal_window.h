#pragma once

#include <QElapsedTimer>
#include <QMainWindow>
#include <QTimer>

class FractalApp;

namespace ui {

class RendererWidget;
class SettingsWidget;

class FractalWindow : public QMainWindow {
  Q_OBJECT
 public:
  explicit FractalWindow(FractalApp* app);

  FractalApp* app();

 private:
  FractalApp* app_ = nullptr;

  QTimer update_timer_;
  QElapsedTimer frame_timer_;

  RendererWidget* renderer_widget_ = nullptr;
  QDockWidget* settings_dock_ = nullptr;
  SettingsWidget* settings_widget_ = nullptr;
};

}  // namespace ui