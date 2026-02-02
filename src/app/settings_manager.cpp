#include "app/settings_manager.h"

SettingsManager::SettingsManager() = default;

void SettingsManager::AddObserver(std::function<void()> observer) {
  observers_.push_back(observer);
}

void SettingsManager::Zoom(double factor) {
  if (factor <= 0.0) {
    return;
  }

  pending_.camera.scale /= factor;

  need_commit_ = true;
}

void SettingsManager::Move(double x, double y, double z) {
  const double dx = x * pending_.camera.scale;
  const double dy = y * pending_.camera.scale;
  const double dz = z * pending_.camera.scale;

  pending_.camera.position.x += dx;
  pending_.camera.position.y += dy;
  pending_.camera.position.z += dz;

  need_commit_ = true;
}

void SettingsManager::Resize(uint32_t w, uint32_t h) {
  if (w == 0 || h == 0) return;

  pending_.camera.aspect = static_cast<double>(w) / h;

  need_commit_ = true;
}

void SettingsManager::SetFractalType(uint8_t type) {
  pending_ = render::RenderSettings{};
  pending_.fractal.type = render::FractalType(type);
  need_commit_ = true;
}

void SettingsManager::SetMaxIterations(uint32_t iterations) {
  pending_.fractal.max_iterations = iterations;
  need_commit_ = true;
}

void SettingsManager::SetJuliaParams(render::JuliaParams params) {
  pending_.fractal.julia = params;
  need_commit_ = true;
}

void SettingsManager::Commit() {
  if (!need_commit_) {
    return;
  }

  settings_ = pending_;
  need_commit_ = false;
  NotifyObservers();
}

render::RenderSettings SettingsManager::GetSettings() { return settings_; }

void SettingsManager::NotifyObservers() {
  for (auto& observer : observers_) {
    observer();
  }
}
