#include "app/settings_manager.h"

#include "render/common/fractals.h"


namespace {

constexpr Vector3d kWorldUp{0.0, 0.0, 1.0};

Vector3d RotateAroundAxis(const Vector3d& v, const Vector3d& axis,
                          double angle) {
  const double c = std::cos(angle);
  const double s = std::sin(angle);

  return v * c + Cross(axis, v) * s + axis * Dot(axis, v) * (1.0 - c);
}

}  // namespace

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

  pending_.camera.position.z += dz;

  const auto right = Normalize(Cross(pending_.camera.direction, kWorldUp));
  pending_.camera.position =
      pending_.camera.position + pending_.camera.direction * dy + right * dx;

  need_commit_ = true;
}

void SettingsManager::RotateCamera(double yaw, double pitch) {
  if (render::Is2DFractal(pending_.fractal.type)) {
    return;
  }

  auto dir = pending_.camera.direction;

  dir = RotateAroundAxis(dir, kWorldUp, yaw);

  const auto right = Normalize(Cross(pending_.camera.direction, kWorldUp));
  dir = RotateAroundAxis(dir, right, pitch);

  if (std::abs(dir.z) > 0.9999) {
    dir.z = std::copysign(0.9999, dir.z);
    const double xy = std::sqrt(1.0 - dir.z * dir.z);
    const double len_xy = std::sqrt(dir.x * dir.x + dir.y * dir.y);

    if (len_xy > 1e-8) {
      dir.x *= xy / len_xy;
      dir.y *= xy / len_xy;
    }
  }

  pending_.camera.direction = Normalize(dir);
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
