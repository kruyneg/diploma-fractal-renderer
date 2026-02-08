#pragma once

#include "render/common/fractals.h"
#include "render/common/types.h"
#include "render/settings_provider.h"

namespace render {

MAYBE_DEVICE inline Vector3d PixelToPosition(int x, int y, uint32_t width,
                                             uint32_t height,
                                             const CameraSettings& cam) {
  double u = (static_cast<double>(x) + 0.5) / width * 2.0 - 1.0;
  double v = (static_cast<double>(y) + 0.5) / height * 2.0 - 1.0;

  u *= cam.aspect;

  Vector3d p = cam.position + Vector3d{1.0, 0.0, 0.0} * (u * cam.scale) +
               Vector3d{0.0, 1.0, 0.0} * (v * cam.scale);

  return {p.x, p.y};
}

MAYBE_DEVICE inline Ray MakeRay(int x, int y, uint32_t width, uint32_t height,
                                const CameraSettings& cam) {
  double u = (x + 0.5) / width * 2.0 - 1.0;
  double v = -((y + 0.5) / height * 2.0 - 1.0);
  u *= cam.aspect;

  const auto right = Normalize(Cross(cam.direction, {0.0, 0.0, 1.0}));
  const auto up = Normalize(Cross(cam.direction, right));

  Vector3d dir = Normalize(right * u + up * v + Normalize(cam.direction));

  return {cam.position, dir};
}

MAYBE_DEVICE inline double CalculateSignedDistance(
    const Vector3d& position, const RenderSettings& settings) {
  switch (settings.fractal.type) {
    case FractalType::kMengerSponge:
      return MengerSpongeSDF(position, settings.fractal.max_iterations);
    case FractalType::kMandelbulb:
      return MandelbulbSDF(position, settings.fractal.max_iterations,
                           settings.fractal.mandelbulb.power,
                           settings.fractal.mandelbulb.boilout);
    default:
      return 100.0;
  }
}

MAYBE_DEVICE inline Vector3d GetNormal(const Vector3d& position,
                                       const RenderSettings& settings) {
  constexpr double eps = 1e-3;
  double dx =
      CalculateSignedDistance(position + Vector3d{eps, 0.0, 0.0}, settings) -
      CalculateSignedDistance(position - Vector3d{eps, 0.0, 0.0}, settings);
  double dy =
      CalculateSignedDistance(position + Vector3d{0.0, eps, 0.0}, settings) -
      CalculateSignedDistance(position - Vector3d{0.0, eps, 0.0}, settings);
  double dz =
      CalculateSignedDistance(position + Vector3d{0.0, 0.0, eps}, settings) -
      CalculateSignedDistance(position - Vector3d{0.0, 0.0, eps}, settings);

  return Normalize(Vector3d{dx, dy, dz} / eps);
}

}  // namespace render
