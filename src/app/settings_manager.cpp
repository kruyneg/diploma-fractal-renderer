#include "app/settings_manager.h"

SettingsManager::SettingsManager() = default;

void SettingsManager::AddObserver(std::function<void()> observer) {
  observers_.push_back(observer);
}

void SettingsManager::Zoom(double factor) {
  if (factor <= 0) {
    return;
  }

  const double center_x = (settings_.view.min_x + settings_.view.max_x) * 0.5;
  const double center_y = (settings_.view.min_y + settings_.view.max_y) * 0.5;

  const double half_w =
      (settings_.view.max_x - settings_.view.min_x) * 0.5 / factor;
  const double half_h =
      (settings_.view.max_y - settings_.view.min_y) * 0.5 / factor;

  settings_.view.min_x = center_x - half_w;
  settings_.view.max_x = center_x + half_w;
  settings_.view.min_y = center_y - half_h;
  settings_.view.max_y = center_y + half_h;

  for (auto& observer : observers_) {
    observer();
  }
}

void SettingsManager::Move(double x, double y) {
  const double width = settings_.view.max_x - settings_.view.min_x;
  const double height = settings_.view.max_y - settings_.view.min_y;

  const double dx = x * width;
  const double dy = y * height;

  settings_.view.min_x += dx;
  settings_.view.max_x += dx;
  settings_.view.min_y += dy;
  settings_.view.max_y += dy;

  for (auto& observer : observers_) {
    observer();
  }
}

void SettingsManager::Resize(uint32_t w, uint32_t h) {
  if (w == 0 || h == 0) return;

  const double center_x = (settings_.view.min_x + settings_.view.max_x) * 0.5;
  const double center_y = (settings_.view.min_y + settings_.view.max_y) * 0.5;

  double half_width = (settings_.view.max_x - settings_.view.min_x) * 0.5;
  double half_height = (settings_.view.max_y - settings_.view.min_y) * 0.5;

  double aspect = static_cast<double>(w) / static_cast<double>(h);

  double current_aspect = half_width / half_height;

  if (current_aspect > aspect) {
    half_height = half_width / aspect;
  } else {
    half_width = half_height * aspect;
  }

  settings_.view.min_x = center_x - half_width;
  settings_.view.max_x = center_x + half_width;
  settings_.view.min_y = center_y - half_height;
  settings_.view.max_y = center_y + half_height;

  for (auto& observer : observers_) {
    observer();
  }
}

void SettingsManager::SetFractalType(uint8_t type) {
  settings_.fractal.type = render::FractalType(type);

  for (auto& observer : observers_) {
    observer();
  }
}

render::RenderSettings SettingsManager::GetSettings() { return settings_; }
