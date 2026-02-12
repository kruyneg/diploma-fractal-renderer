#pragma once

#include <functional>
#include <vector>

#include "render/settings_provider.h"

class SettingsManager : public render::SettingsProvider {
 public:
  SettingsManager();

  void AddObserver(std::function<void()> listener);
  void Zoom(float factor);
  void Move(float x, float y, float z);
  void RotateCamera(float yaw, float pitch);
  void Resize(uint32_t w, uint32_t h);
  void SetFractalType(uint8_t type);
  void SetMaxIterations(uint32_t iterations);
  void SetJuliaParams(render::JuliaParams params);
  void SetMandelbulbParams(render::MandelbulbParams params);
  void SetMandelboxParams(render::MandelboxParams params);
  void SetJuliabulbParams(render::JuliabulbParams params);

  void Commit();

  render::RenderSettings GetSettings() override;

 private:
  void NotifyObservers();

  render::RenderSettings pending_;
  bool need_commit_ = false;

  render::RenderSettings settings_;

  std::vector<std::function<void()>> observers_;
};
