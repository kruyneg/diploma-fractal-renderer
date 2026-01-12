#include "app/settings_manager.h"

#include <algorithm>

namespace {
constexpr uint32_t kMaxIterations = 512;
constexpr uint32_t kMinIterations = 128;
}  // namespace

SettingsManager::SettingsManager() {
  settings_.min_x = -2.5;
  settings_.max_x = 1.0;
  settings_.min_y = -1.0;
  settings_.max_y = 1.0;
  settings_.max_iter = kMaxIterations;
}

void SettingsManager::AddObserver(std::function<void()> observer) {
  observers_.push_back(observer);
}

void SettingsManager::Zoom(double factor) {
  if (factor <= 0) {
    return;
  }

  const double center_x = (settings_.min_x + settings_.max_x) * 0.5;
  const double center_y = (settings_.min_y + settings_.max_y) * 0.5;

  const double half_w = (settings_.max_x - settings_.min_x) * 0.5 / factor;
  const double half_h = (settings_.max_y - settings_.min_y) * 0.5 / factor;

  settings_.min_x = center_x - half_w;
  settings_.max_x = center_x + half_w;
  settings_.min_y = center_y - half_h;
  settings_.max_y = center_y + half_h;

  for (auto& observer : observers_) {
    observer();
  }
}

void SettingsManager::Move(double x, double y) {
  const double width = settings_.max_x - settings_.min_x;
  const double height = settings_.max_y - settings_.min_y;

  const double dx = x * width;
  const double dy = y * height;

  settings_.min_x += dx;
  settings_.max_x += dx;
  settings_.min_y += dy;
  settings_.max_y += dy;

  for (auto& observer : observers_) {
    observer();
  }
}

void SettingsManager::Resize(uint32_t w, uint32_t h) {}

render::RenderSettings SettingsManager::GetSettings() { return settings_; }
