#pragma once

#include <QMainWindow>

class FractalApp;

namespace ui {

class RendererWidget;
class InputController;

class FractalWindow : public QMainWindow {
  Q_OBJECT
 public:
  explicit FractalWindow(FractalApp* app);

 protected:
  void resizeEvent(QResizeEvent* event) override;
  void keyPressEvent(QKeyEvent* event) override;

 private:
  FractalApp* app_ = nullptr;

  RendererWidget* renderer_widget_ = nullptr;
  InputController* input_controller_ = nullptr;
};

}  // namespace ui