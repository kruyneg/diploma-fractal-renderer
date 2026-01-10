#pragma once

#include <QMainWindow>

#include "render/renderer.h"

namespace ui {

class RendererWidget;

class FractalWindow : public QMainWindow {
  Q_OBJECT
 public:
  explicit FractalWindow(Renderer* renderer);

 private:
  RendererWidget* renderer_widget_ = nullptr;
};

}  // namespace ui