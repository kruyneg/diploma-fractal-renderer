#include "render/cpu/cpu_renderer.h"

#include "render/texture_target.h"

CPURenderer::CPURenderer() = default;

void CPURenderer::Resize(uint32_t w, uint32_t h) {
  width_ = w;
  height_ = h;

  buffer_.resize(w * h);
}

void CPURenderer::Render(TextureTarget* target) {
  if (!target || width_ == 0 || height_ == 0) {
    return;
  }

  const float cx = width_ * 0.5f;
  const float cy = height_ * 0.5f;
  const float radius = std::min(width_, height_) * 0.35f;
  const float r2 = radius * radius;

  for (uint32_t y = 0; y < height_; ++y) {
    for (uint32_t x = 0; x < width_; ++x) {
      const float dx = x - cx;
      const float dy = y - cy;
      const float dist2 = dx * dx + dy * dy;

      Color& c = buffer_[y * width_ + x];

      if (dist2 <= r2) {
        c = {255, 80, 80, 255};
      } else {
        c = {30, 30, 30, 255};
      }
    }
  }

  UploadBufferToTexture(target);
}

void CPURenderer::UploadBufferToTexture(TextureTarget* target) const {
  if (!target) {
    return;
  }

  QOpenGLFunctions* gl = QOpenGLContext::currentContext()->functions();

  gl->glBindTexture(GL_TEXTURE_2D, target->texture());

  gl->glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

  gl->glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width_, height_, GL_RGBA,
                      GL_UNSIGNED_BYTE, buffer_.data());

  gl->glBindTexture(GL_TEXTURE_2D, 0);
}