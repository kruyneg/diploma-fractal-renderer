#include "app/settings_manager.h"

SettingsManager::SettingsManager() = default;

void SettingsManager::AddObserver(std::function<void()> observer) {
  observers_.push_back(observer);
}

void SettingsManager::Zoom(double factor) {
  if (factor <= 0) {
    return;
  }

  const double center_x = (pending_.view.min_x + pending_.view.max_x) * 0.5;
  const double center_y = (pending_.view.min_y + pending_.view.max_y) * 0.5;

  const double half_w =
      (pending_.view.max_x - pending_.view.min_x) * 0.5 / factor;
  const double half_h =
      (pending_.view.max_y - pending_.view.min_y) * 0.5 / factor;

  pending_.view.min_x = center_x - half_w;
  pending_.view.max_x = center_x + half_w;
  pending_.view.min_y = center_y - half_h;
  pending_.view.max_y = center_y + half_h;

  need_commit_ = true;
}

void SettingsManager::Move(double x, double y) {
  const double width = pending_.view.max_x - pending_.view.min_x;
  const double height = pending_.view.max_y - pending_.view.min_y;

  const double dx = x * width;
  const double dy = y * height;

  pending_.view.min_x += dx;
  pending_.view.max_x += dx;
  pending_.view.min_y += dy;
  pending_.view.max_y += dy;

  need_commit_ = true;
}

void SettingsManager::Resize(uint32_t w, uint32_t h) {
  if (w == 0 || h == 0) return;

  const double center_x = (pending_.view.min_x + pending_.view.max_x) * 0.5;
  const double center_y = (pending_.view.min_y + pending_.view.max_y) * 0.5;

  double half_width = (pending_.view.max_x - pending_.view.min_x) * 0.5;
  double half_height = (pending_.view.max_y - pending_.view.min_y) * 0.5;

  double aspect = static_cast<double>(w) / static_cast<double>(h);

  double current_aspect = half_width / half_height;

  half_width = half_height * aspect;

  pending_.view.min_x = center_x - half_width;
  pending_.view.max_x = center_x + half_width;
  pending_.view.min_y = center_y - half_height;
  pending_.view.max_y = center_y + half_height;

  need_commit_ = true;
}

void SettingsManager::SetFractalType(uint8_t type) {
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
