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

MAYBE_DEVICE inline int MandelbrotIterations(float x, float y, int max_iter) {
  float zr = 0.0f;
  float zi = 0.0f;
  int i = 0;

  while (zr * zr + zi * zi <= 4.0 && i < max_iter) {
    float tmp = zr * zr - zi * zi + x;
    zi = 2.0f * zr * zi + y;
    zr = tmp;
    ++i;
  }
  return i;
}

MAYBE_DEVICE inline int JuliaIterations(float x, float y, int max_iter,
                                        float c_re = -0.8, float c_im = 0.156) {
  int i = 0;

  while (x * x + y * y <= 4.0 && i < max_iter) {
    const float x_new = x * x - y * y + c_re;
    y = 2.0f * x * y + c_im;
    x = x_new;

    ++i;
  }
  return i;
}

MAYBE_DEVICE inline float BoxSDF(const Vector3d& p, const Vector3d& b) {
  auto q = Abs(p) - b;
  return Length({fmax(q.x, 0.0f), fmax(q.y, 0.0f), fmax(q.z, 0.0f)}) +
         fmin(fmax(q.x, fmax(q.y, q.z)), 0.0f);
}

MAYBE_DEVICE inline float MengerSpongeSDF(Vector3d pos, int iterations) {
  float d = BoxSDF(pos, {1.0f, 1.0f, 1.0f});
  const auto z = Abs(pos);

  float scale = 1.0f;
  for (int m = 0; m < iterations; m++) {
    Vector3d a = {fmod(z.x * scale, 2.0f) - 1.0f,
                  fmod(z.y * scale, 2.0f) - 1.0f,
                  fmod(z.z * scale, 2.0f) - 1.0f};
    scale *= 3.0;
    Vector3d r = Abs(Vector3d{1.0f, 1.0f, 1.0f} - Abs(a) * 3.0);

    float da = fmax(r.x, r.y);
    float db = fmax(r.y, r.z);
    float dc = fmax(r.z, r.x);
    float c = (fmin(da, fmin(db, dc)) - 1.0f) / scale;

    d = fmax(d, c);
  }

  return d;
}

MAYBE_DEVICE inline float MandelbulbSDF(const Vector3d& pos, int iterations,
                                        float power = 8.0,
                                        float bailout = 2.0f) {
  Vector3d z = pos;
  float dr = 1.0f;
  float r;

  for (int i = 0; i < iterations; ++i) {
    r = Length(z);
    if (r > bailout) {
      break;
    }

    float theta = acos(z.z / r);
    float phi = atan2(z.y, z.x);

    float zr = pow(r, power - 1.0f);
    dr = zr * power * dr + 1.0f;
    zr *= r;
    theta *= power;
    phi *= power;

    z = {zr * sin(theta) * cos(phi), zr * sin(theta) * sin(phi),
         zr * cos(theta)};

    z = z + pos;
  }

  return 0.5 * log(r) * r / dr;
}

MAYBE_DEVICE inline float MandelboxSDF(const Vector3d& pos, int iterations,
                                       float min_radius, float fixed_radius,
                                       float scale) {
  Vector3d z = pos;
  float dr = 1.0f;

  const float min_r2 = min_radius * min_radius;
  const float fixed_r2 = fixed_radius * fixed_radius;

  for (int i = 0; i < iterations; ++i) {
    auto clamped = z;
    clamped.x = fmin(fmax(clamped.x, -1.0f), 1.0f);
    clamped.y = fmin(fmax(clamped.y, -1.0f), 1.0f);
    clamped.z = fmin(fmax(clamped.z, -1.0f), 1.0f);
    z = clamped * 2.0f - z;

    const float r2 = Dot(z, z);
    if (r2 < min_r2) {
      const float factor = fixed_r2 / min_r2;
      z = z * factor;
      dr = dr * factor;
    } else if (r2 < fixed_r2) {
      const float factor = fixed_r2 / r2;
      z = z * factor;
      dr = dr * factor;
    }

    z = z * scale + pos;
    dr = dr * fabs(scale) + 1.0f;

    if (Length(z) > 100.0f) {
      break;
    }
  }

  return Length(z) / dr;
}

MAYBE_DEVICE inline float JuliabulbSDF(const Vector3d& p, int max_iter,
                                       const Vector3d& c = {0.1, 1.0, 0.0},
                                       float power = 5.5) {
  const float bailout = 2.0f;

  Vector3d z = p;
  float dr = 1.0f;
  float r = 0.0f;
  for (int i = 0; i < max_iter; ++i) {
    r = Length(z);
    if (r > bailout) break;

    float r_pow = powf(r, power - 1.0f);
    dr = r_pow * power * dr + 1.0f;

    float theta = acosf(fmin(fmax(z.z / r, -1.0f), 1.0f));
    float phi = atan2f(z.y, z.x);

    theta *= power;
    phi *= power;

    const float sin_theta = sinf(theta);
    const float cos_theta = cosf(theta);
    const float cos_phi = cosf(phi);
    const float sin_phi = sinf(phi);
    r_pow *= r;
    z = Vector3d{r_pow * sin_theta * cos_phi, r_pow * cos_theta,
                 r_pow * sin_theta * sin_phi};

    z = z + c;
  }

  return 0.5f * logf(r) * r / dr;
}

}  // namespace render
