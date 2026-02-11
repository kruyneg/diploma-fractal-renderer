#pragma once

#include "render/common/fractals.h"
#include "render/common/types.h"
#include "render/settings_provider.h"

namespace render {

MAYBE_DEVICE inline Vector3d PixelToPosition(int x, int y, uint32_t width,
                                             uint32_t height,
                                             const CameraSettings& cam) {
  float u = (static_cast<float>(x) + 0.5) / width * 2.0f - 1.0f;
  float v = (static_cast<float>(y) + 0.5) / height * 2.0f - 1.0f;

  u *= cam.aspect;

  Vector3d p = cam.position + Vector3d{1.0f, 0.0f, 0.0f} * (u * cam.scale) +
               Vector3d{0.0f, 1.0f, 0.0f} * (v * cam.scale);

  return {p.x, p.y};
}

MAYBE_DEVICE inline Ray MakeRay(int x, int y, uint32_t width, uint32_t height,
                                const CameraSettings& cam) {
  float u = (x + 0.5) / width * 2.0f - 1.0f;
  float v = -((y + 0.5) / height * 2.0f - 1.0f);
  u *= cam.aspect;

  const auto right = Normalize(Cross(cam.direction, {0.0f, 0.0f, 1.0f}));
  const auto up = Normalize(Cross(cam.direction, right));

  Vector3d dir = Normalize(right * u + up * v + Normalize(cam.direction));

  return {cam.position, dir};
}

MAYBE_DEVICE inline float CalculateSignedDistance(
    const Vector3d& position, const RenderSettings& settings) {
  switch (settings.fractal.type) {
    case FractalType::kMengerSponge:
      return MengerSpongeSDF(position, settings.fractal.max_iterations);
    case FractalType::kMandelbulb:
      return MandelbulbSDF(position, settings.fractal.max_iterations,
                           settings.fractal.mandelbulb.power,
                           settings.fractal.mandelbulb.boilout);
    case FractalType::kMandelbox:
      return MandelboxSDF(position, settings.fractal.max_iterations,
                          settings.fractal.mandelbox.min_radius,
                          settings.fractal.mandelbox.fixed_radius,
                          settings.fractal.mandelbox.scale);
    default:
      return 100.0;
  }
}

MAYBE_DEVICE inline Vector3d GetNormal(const Vector3d& position,
                                       const RenderSettings& settings,
                                       float eps = 1e-3) {
  float dx =
      CalculateSignedDistance(position + Vector3d{eps, 0.0f, 0.0f}, settings) -
      CalculateSignedDistance(position - Vector3d{eps, 0.0f, 0.0f}, settings);
  float dy =
      CalculateSignedDistance(position + Vector3d{0.0f, eps, 0.0f}, settings) -
      CalculateSignedDistance(position - Vector3d{0.0f, eps, 0.0f}, settings);
  float dz =
      CalculateSignedDistance(position + Vector3d{0.0f, 0.0f, eps}, settings) -
      CalculateSignedDistance(position - Vector3d{0.0f, 0.0f, eps}, settings);

  return Normalize(Vector3d{dx, dy, dz} / eps);
}

}  // namespace render
