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
  void SetFractalType(uint8_t type);

  render::RenderSettings GetSettings() override;

 private:
  render::RenderSettings settings_;

  std::vector<std::function<void()>> observers_;
};
