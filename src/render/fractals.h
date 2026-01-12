#pragma once

#include <cstdint>

#include "render/utils.h"

#ifdef __CUDACC__
#define MAYBE_DEVICE __host__ __device__
#else
#define MAYBE_DEVICE
#endif

namespace render {

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

}  // namespace render
