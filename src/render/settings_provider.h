#pragma once

#include <cstdint>

#include "render/common/types.h"

namespace render {

enum class FractalType : uint8_t {
  kMandelbrot,
  kJulia,
  kMengerSponge,
};

struct JuliaParams {
  double c_re = -0.8;
  double c_im = 0.156;
};

struct FractalSettings {
  FractalType type = FractalType::kMandelbrot;
  uint32_t max_iterations = 5;

  JuliaParams julia;
};

struct CameraSettings {
  Vector3d position = {0.0, 0.0, 0.0};
  Vector3d direction = {0.0, 1.0, 0.0};
  double scale = 1.0;
  double aspect = 1.0;
};

struct RenderSettings {
  CameraSettings camera;
  FractalSettings fractal;
};

class SettingsProvider {
 public:
  virtual RenderSettings GetSettings() = 0;
};

}  // namespace render
