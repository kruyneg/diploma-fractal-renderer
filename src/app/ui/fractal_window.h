#pragma once

#include <QElapsedTimer>
#include <QMainWindow>
#include <QTimer>

class FractalApp;

namespace ui {

class RendererWidget;
class SettingsWidget;
class InputController;

class FractalWindow : public QMainWindow {
  Q_OBJECT
 public:
  explicit FractalWindow(FractalApp* app);

 protected:
  void keyPressEvent(QKeyEvent* event) override;
  void keyReleaseEvent(QKeyEvent* event) override;

 private:
  bool ShouldHandleInput() const;

  FractalApp* app_ = nullptr;

  QTimer update_timer_;
  QElapsedTimer frame_timer_;

  RendererWidget* renderer_widget_ = nullptr;
  QDockWidget* settings_dock_ = nullptr;
  SettingsWidget* settings_widget_ = nullptr;

  InputController* input_controller_ = nullptr;
};

}  // namespace ui