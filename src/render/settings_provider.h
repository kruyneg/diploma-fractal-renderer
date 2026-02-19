#pragma once

#include <cstdint>

#include "render/common/types.h"

namespace render {

enum class FractalType : uint8_t {
  kMandelbrot,
  kJulia,
  kMengerSponge,
  kMandelbulb,
  kMandelbox,
  kJuliabulb,
};

struct JuliaParams {
  float c_re = -0.8;
  float c_im = 0.156;
};

struct MandelbulbParams {
  float power = 8.0;
  float boilout = 2.0;
};

struct MandelboxParams {
  float min_radius = 0.5;
  float fixed_radius = 1.0;
  float scale = 2.0;
};

struct JuliabulbParams {
  Vector3d c = {0.1, 1.0, 0.0};
  float power = 8.0;
};

struct FractalSettings {
  FractalType type = FractalType::kMandelbrot;
  uint32_t max_iterations = 5;

  JuliaParams julia;
  MandelbulbParams mandelbulb;
  MandelboxParams mandelbox;
  JuliabulbParams juliabulb;
};

struct CameraSettings {
  Vector3d position = {0.0f, -2.0f, 0.0f};
  Vector3d direction = {0.0f, 1.0f, 0.0f};
  float scale = 1.0f;
  float aspect = 1.0f;
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
