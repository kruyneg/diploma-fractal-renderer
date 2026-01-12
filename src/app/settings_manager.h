#pragma once

#include <functional>
#include <vector>

#include "render/settings_provider.h"

class SettingsManager : public render::SettingsProvider {
 public:
  SettingsManager();

  void AddObserver(std::function<void()> listener);
  void Zoom(double factor);
  void Move(double x, double y);
  void Resize(uint32_t w, uint32_t h);

  render::RenderSettings GetSettings() override;

 private:
  render::RenderSettings settings_;
  double aspect_ratio_ = 1.0;

  std::vector<std::function<void()>> observers_;
};
