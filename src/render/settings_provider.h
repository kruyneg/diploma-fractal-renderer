#pragma once

#include <cstdint>

namespace render {

enum class FractalType : uint8_t {
  kMandelbrot,
  kJulia,
};

struct JuliaParams {
  double c_re = -0.8;
  double c_im = 0.156;
};

struct FractalSettings {
  FractalType type = FractalType::kMandelbrot;
  uint32_t max_iterations = 128;

  JuliaParams julia;
};

struct ViewSettings {
  double min_x = -1.0;
  double min_y = -1.0;
  double max_x = 1.0;
  double max_y = 1.0;
};

struct RenderSettings {
  ViewSettings view;
  FractalSettings fractal;
};

class SettingsProvider {
 public:
  virtual RenderSettings GetSettings() = 0;
};

}  // namespace render
