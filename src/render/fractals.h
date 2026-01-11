#pragma once

#include <cstdint>

#include "render/utils.h"

#ifdef __CUDACC__
#define MAYBE_DEVICE __host__ __device__
#else
#define MAYBE_DEVICE
#endif

struct Complex {
  double re;
  double im;
};

MAYBE_DEVICE inline int MandelbrotIterations(Complex c, int max_iter) {
  double zr = 0.0;
  double zi = 0.0;
  int i = 0;

  while (zr * zr + zi * zi <= 4.0 && i < max_iter) {
    double tmp = zr * zr - zi * zi + c.re;
    zi = 2.0 * zr * zi + c.im;
    zr = tmp;
    ++i;
  }
  return i;
}

MAYBE_DEVICE inline Color ColorFromIter(int iter, int max_iter) {
  if (iter == max_iter) {
    return Color{0, 0, 0, 255};
  }

  uint8_t v = static_cast<uint8_t>(255.0 * iter / max_iter);
  return Color{v, v, v, 255};
}
