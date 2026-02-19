#pragma once

#include <cmath>
#include <cstdint>

#ifdef __CUDACC__
#define MAYBE_DEVICE __host__ __device__
#else
#define MAYBE_DEVICE
#endif

struct Color {
  uint8_t r;
  uint8_t g;
  uint8_t b;
  uint8_t a;
};

struct Vector3d {
  float x;
  float y;
  float z;
};

struct Ray {
  Vector3d position;
  Vector3d direction;
};

MAYBE_DEVICE inline Vector3d operator*(const Vector3d& vec, float scalar) {
  return {vec.x * scalar, vec.y * scalar, vec.z * scalar};
}

MAYBE_DEVICE inline Vector3d operator/(const Vector3d& vec, float scalar) {
  return {vec.x / scalar, vec.y / scalar, vec.z / scalar};
}

MAYBE_DEVICE inline Vector3d operator+(const Vector3d& a, const Vector3d& b) {
  return {a.x + b.x, a.y + b.y, a.z + b.z};
}

MAYBE_DEVICE inline Vector3d operator-(const Vector3d& a, const Vector3d& b) {
  return {a.x - b.x, a.y - b.y, a.z - b.z};
}

MAYBE_DEVICE inline Vector3d operator-(const Vector3d& vec) {
  return {-vec.x, -vec.y, -vec.z};
}

MAYBE_DEVICE inline Vector3d operator*(const Vector3d& a, const Vector3d& b) {
  return {a.x * b.x, a.y * b.y, a.z * b.z};
}

MAYBE_DEVICE inline float Dot(const Vector3d& a, const Vector3d& b) {
  return a.x * b.x + a.y * b.y + a.z * b.z;
}

MAYBE_DEVICE inline Vector3d Cross(const Vector3d& a, const Vector3d& b) {
  return {a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x};
}

MAYBE_DEVICE inline float Length(const Vector3d& vec) {
  return sqrt(Dot(vec, vec));
}

MAYBE_DEVICE inline Vector3d Normalize(const Vector3d& vec) {
  return vec / Length(vec);
}

MAYBE_DEVICE inline Vector3d Abs(const Vector3d& vec) {
  return {fabs(vec.x), fabs(vec.y), fabs(vec.z)};
}

MAYBE_DEVICE inline Vector3d Fract(const Vector3d& vec) {
  return {vec.x - floor(vec.x), vec.y - floor(vec.y), vec.z - floor(vec.z)};
}

MAYBE_DEVICE inline float MaxComponent(const Vector3d& vec) {
  return fmax(vec.x, fmax(vec.y, vec.z));
}
