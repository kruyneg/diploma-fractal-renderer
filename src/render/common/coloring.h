#pragma once

#include "render/common/types.h"
#include "render/settings_provider.h"

namespace render {

MAYBE_DEVICE inline Color ColorFromIter(
    int iter, int max_iter, Color target = Color{255, 255, 255, 255},
    Color background = Color{0, 0, 0, 255}) {
  if (iter == max_iter) {
    return Color{0, 0, 0, 255};
  }

  float t = static_cast<float>(iter) / static_cast<float>(max_iter);

  uint8_t r = static_cast<uint8_t>(t * target.r + (1.0f - t) * background.r);
  uint8_t g = static_cast<uint8_t>(t * target.g + (1.0f - t) * background.g);
  uint8_t b = static_cast<uint8_t>(t * target.b + (1.0f - t) * background.b);

  return Color{r, g, b, 255};
}

MAYBE_DEVICE inline float MandelbulbOrbit(Vector3d pos,
                                          const FractalSettings& s) {
  Vector3d z = pos;
  float dr = 1.0f;
  float r = 0.0f;

  float orbit = 1e20f;

  for (int i = 0; i < s.max_iterations; i++) {
    r = Length(z);
    if (r > s.mandelbulb.boilout) break;

    orbit = fminf(orbit, Length(z));
    float theta = acosf(z.z / r);
    float phi = atan2f(z.y, z.x);
    dr = powf(r, s.mandelbulb.power - 1.0f) * s.mandelbulb.power * dr + 1.0f;

    float zr = powf(r, s.mandelbulb.power);
    theta *= s.mandelbulb.power;
    phi *= s.mandelbulb.power;

    z = Vector3d(sinf(theta) * cosf(phi), sinf(phi) * sinf(theta),
                 cosf(theta)) *
        zr;

    z = z + pos;
  }

  float dist = 0.5f * logf(r) * r / dr;

  return orbit;
}

MAYBE_DEVICE inline float JuliabulbOrbit(const Vector3d& p,
                                         const FractalSettings& settings) {
  const float bailout = 2.0f;

  float orbit = 1e20f;

  Vector3d z = p;
  float dr = 1.0f;
  float r = 0.0f;
  for (int i = 0; i < settings.max_iterations; ++i) {
    r = Length(z);
    if (r > bailout) break;

    orbit = fminf(orbit, r);

    float r_pow = powf(r, settings.juliabulb.power - 1.0f);
    dr = r_pow * settings.juliabulb.power * dr + 1.0f;

    float theta = acosf(fmin(fmax(z.z / r, -1.0f), 1.0f));
    float phi = atan2f(z.y, z.x);

    theta *= settings.juliabulb.power;
    phi *= settings.juliabulb.power;

    const float sin_theta = sinf(theta);
    const float cos_theta = cosf(theta);
    const float cos_phi = cosf(phi);
    const float sin_phi = sinf(phi);
    r_pow *= r;
    z = Vector3d{r_pow * sin_theta * cos_phi, r_pow * cos_theta,
                 r_pow * sin_theta * sin_phi};

    z = z + settings.juliabulb.c;
  }

  return orbit;
}

MAYBE_DEVICE inline Color GetOrbitTrapColor(const Vector3d& pos,
                                            const FractalSettings& settings) {
  float orbit;
  if (settings.type == FractalType::kMandelbulb) {
    orbit = MandelbulbOrbit(pos, settings);
  } else if (settings.type == FractalType::kJuliabulb) {
    orbit = JuliabulbOrbit(pos, settings);
  }
  orbit = fmaxf(orbit, 1e-6f) * 0.5f;

  const float v = -logf(orbit);

  const float r = 0.5f + 0.5f * cosf(6.2831f * (v + 0.0f));
  const float g = 0.5f + 0.5f * cosf(6.2831f * (v + 0.33f));
  const float b = 0.5f + 0.5f * cosf(6.2831f * (v + 0.67f));

  return Color{
      static_cast<uint8_t>(255.0f * r),
      static_cast<uint8_t>(255.0f * g),
      static_cast<uint8_t>(255.0f * b),
      255,
  };
}

MAYBE_DEVICE inline Color GetMandelboxColor(const Vector3d& pos) {
  const float len = Length(pos);
  const float t = 1.0f - 1.0f / (1.0f + 0.5f * len);

  const float t0 = fminf(t * 2.0f, 1.0f);
  const float t1 = fminf(fmaxf((t - 0.5f) * 2.0f, 0.0f), 1.0f);

  // Red -> Green
  float r = 1.0f - t0;
  float g = t0;
  float b = 0.0f;

  // Green -> Blue
  r = r * (1.0f - t1);
  g = g * (1.0f - t1);
  b = t1;

  return Color{
      static_cast<uint8_t>(255.0f * r),
      static_cast<uint8_t>(255.0f * g),
      static_cast<uint8_t>(255.0f * b),
      255,
  };
}

MAYBE_DEVICE inline Color GetMengerSpongeColor(const Vector3d& normal) {
  const auto v = Abs(normal);
  return Color{
      static_cast<uint8_t>(255.0f * v.x),
      static_cast<uint8_t>(255.0f * v.y),
      static_cast<uint8_t>(255.0f * v.z),
      255,
  };
}

MAYBE_DEVICE inline Color GetFractalColor(const Vector3d& pos,
                                          const Vector3d& normal,
                                          const FractalSettings& settings) {
  switch (settings.type) {
    case FractalType::kMandelbulb:
    case FractalType::kJuliabulb:
      return GetOrbitTrapColor(pos, settings);
    case FractalType::kMandelbox:
      return GetMandelboxColor(pos);
    case FractalType::kMengerSponge:
      return GetMengerSpongeColor(normal);
    default:
      return Color{255, 255, 255, 255};
  }
}

MAYBE_DEVICE inline float Saturate(float x) {
  return fminf(fmaxf(x, 0.0f), 1.0f);
}

MAYBE_DEVICE inline Color Lighting(const Color& base_color, const Vector3d& pos,
                                   const Vector3d& normal,
                                   const RenderSettings& settings) {
  const Vector3d base(base_color.r / 255.0f, base_color.g / 255.0f,
                      base_color.b / 255.0f);

  float ambient = 0.1f;

  const auto light_dir = Normalize(settings.camera.position - pos);
  float diffuse = fmaxf(Dot(normal, light_dir), 0.0f);

  const auto view_dir = Normalize(-settings.camera.direction);
  const auto half_dir = Normalize(light_dir + view_dir);
  float spec = powf(fmaxf(Dot(normal, half_dir), 0.0f), 128.0f);

  auto linear =
      base * (ambient + diffuse) + Vector3d(1.0f, 1.0f, 1.0f) * spec * 0.4f;
  linear.x = powf(Saturate(linear.x), 1.0f / 2.2f);
  linear.y = powf(Saturate(linear.y), 1.0f / 2.2f);
  linear.z = powf(Saturate(linear.z), 1.0f / 2.2f);

  return Color{
      static_cast<uint8_t>(Saturate(linear.x) * 255.0f),
      static_cast<uint8_t>(Saturate(linear.y) * 255.0f),
      static_cast<uint8_t>(Saturate(linear.z) * 255.0f),
      255,
  };
}

}  // namespace render