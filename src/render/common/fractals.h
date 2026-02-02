#pragma once

#include <cstdint>

#include "render/common/types.h"
#include "render/settings_provider.h"

namespace render {

MAYBE_DEVICE inline bool Is2DFractal(FractalType type) {
  switch (type) {
    case FractalType::kMandelbrot:
    case FractalType::kJulia:
      return true;
    default:
      return false;
  }
}

MAYBE_DEVICE inline int MandelbrotIterations(double x, double y, int max_iter) {
  double zr = 0.0;
  double zi = 0.0;
  int i = 0;

  while (zr * zr + zi * zi <= 4.0 && i < max_iter) {
    double tmp = zr * zr - zi * zi + x;
    zi = 2.0 * zr * zi + y;
    zr = tmp;
    ++i;
  }
  return i;
}

MAYBE_DEVICE inline int JuliaIterations(double x, double y, int max_iter,
                                        double c_re = -0.8,
                                        double c_im = 0.156) {
  int i = 0;

  while (x * x + y * y <= 4.0 && i < max_iter) {
    const double x_new = x * x - y * y + c_re;
    y = 2.0 * x * y + c_im;
    x = x_new;

    ++i;
  }
  return i;
}

MAYBE_DEVICE inline Color ColorFromIter(
    int iter, int max_iter, Color target = Color{255, 255, 255, 255},
    Color background = Color{0, 0, 0, 255}) {
  if (iter == max_iter) {
    return Color{0, 0, 0, 255};
  }

  float t = static_cast<float>(iter) / static_cast<float>(max_iter);

  uint8_t r = static_cast<uint8_t>(t * target.r + (1.0 - t) * background.r);
  uint8_t g = static_cast<uint8_t>(t * target.g + (1.0 - t) * background.g);
  uint8_t b = static_cast<uint8_t>(t * target.b + (1.0 - t) * background.b);

  return Color{r, g, b, 255};
}

MAYBE_DEVICE inline double BoxSDF(const Vector3d& p, const Vector3d& b) {
  auto q = Abs(p) - b;
  return Length({fmax(q.x, 0.0), fmax(q.y, 0.0), fmax(q.z, 0.0)}) +
         fmin(fmax(q.x, fmax(q.y, q.z)), 0.0);
}

MAYBE_DEVICE inline double MengerSpongeSDF(Vector3d p, int iterations) {
  double d = BoxSDF(p, {1.0, 1.0, 1.0});
  p = Abs(p);

  double scale = 1.0;
  for (int m = 0; m < iterations; m++) {
    Vector3d a = {fmod(p.x * scale, 2.0) - 1.0, fmod(p.y * scale, 2.0) - 1.0,
                  fmod(p.z * scale, 2.0) - 1.0};
    scale *= 3.0;
    Vector3d r = Abs(Vector3d{1.0, 1.0, 1.0} - Abs(a) * 3.0);

    double da = fmax(r.x, r.y);
    double db = fmax(r.y, r.z);
    double dc = fmax(r.z, r.x);
    double c = (fmin(da, fmin(db, dc)) - 1.0) / scale;

    d = fmax(d, c);
  }

  return d;
}

}  // namespace render
