#pragma once

#include <QOpenGLFunctions>
#include <cstdint>

namespace ui {

class TextureTarget {
 public:
  TextureTarget();
  ~TextureTarget();

  TextureTarget(const TextureTarget&) = delete;
  TextureTarget& operator=(const TextureTarget&) = delete;

  TextureTarget(TextureTarget&& other) noexcept;
  TextureTarget& operator=(TextureTarget&& other) noexcept;

  void Init();
  void Resize(uint32_t width, uint32_t height);
  void Destroy();

  GLuint texture() const;
  uint32_t width() const;
  uint32_t height() const;

 private:
  GLuint texture_id_ = 0;
  uint32_t width_ = 0;
  uint32_t height_ = 0;
};

}  // namespace ui
