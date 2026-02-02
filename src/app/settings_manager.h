#pragma once

#include <functional>
#include <vector>

#include "render/settings_provider.h"

class SettingsManager : public render::SettingsProvider {
 public:
  SettingsManager();

  void AddObserver(std::function<void()> listener);
  void Zoom(double factor);
  void Move(double x, double y, double z);
  void Resize(uint32_t w, uint32_t h);
  void SetFractalType(uint8_t type);
  void SetMaxIterations(uint32_t iterations);
  void SetJuliaParams(render::JuliaParams params);

  void Commit();

  render::RenderSettings GetSettings() override;

 private:
  void NotifyObservers();

  render::RenderSettings pending_;
  bool need_commit_ = false;

  render::RenderSettings settings_;

  std::vector<std::function<void()>> observers_;
};
