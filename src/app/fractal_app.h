#pragma once

#include <memory>

#include "render/renderer.h"

namespace ui {
class FractalWindow;
}

class FractalApp {
 public:
  FractalApp();
  ~FractalApp();

  void Run();

 private:
  std::unique_ptr<ui::FractalWindow> main_window_;
  std::unique_ptr<Renderer> renderer_;
};