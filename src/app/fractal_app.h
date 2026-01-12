#pragma once

#include <memory>

#include "app/settings_manager.h"
#include "render/renderer.h"

namespace ui {
class FractalWindow;
}

class FractalApp {
 public:
  FractalApp();
  ~FractalApp();

  void Run();

  const render::Renderer* renderer() const;
  render::Renderer* renderer();

  const SettingsManager& settings() const;
  SettingsManager& settings();

 private:
  SettingsManager settings_;
  std::unique_ptr<ui::FractalWindow> main_window_;
  std::unique_ptr<render::Renderer> renderer_;
};