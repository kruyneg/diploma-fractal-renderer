#include "render/cpu/cpu_renderer.h"

#include "QOpenGLFunctions"
#include "render/fractals.h"

CPURenderer::CPURenderer() = default;

void CPURenderer::Init(uint32_t target_tex_id) {
  target_ = target_tex_id;
}

void CPURenderer::Resize(uint32_t w, uint32_t h) {
  width_ = w;
  height_ = h;

  buffer_.resize(w * h);
}

void CPURenderer::Render() {
  if (target_ == 0 || width_ == 0 || height_ == 0) {
    return;
  }

  const int max_iter = 128;

  const double min_re = -2.5;
  const double max_re = 1.0;
  const double min_im = -1.0;
  const double max_im = 1.0;

  const double scale_re = (max_re - min_re) / static_cast<double>(width_);
  const double scale_im = (max_im - min_im) / static_cast<double>(height_);

  for (uint32_t y = 0; y < height_; ++y) {
    double c_im = max_im - y * scale_im;

    for (uint32_t x = 0; x < width_; ++x) {
      double c_re = min_re + x * scale_re;

      const int iteration = MandelbrotIterations({c_re, c_im}, max_iter);
      Color& c = buffer_[y * width_ + x];

      c = ColorFromIter(iteration, max_iter);
    }
  }

  UploadBufferToTarget();
}

void CPURenderer::UploadBufferToTarget() const {
  if (target_ == 0) {
    return;
  }

  QOpenGLFunctions* gl = QOpenGLContext::currentContext()->functions();

  gl->glBindTexture(GL_TEXTURE_2D, target_);

  gl->glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

  gl->glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width_, height_, GL_RGBA,
                      GL_UNSIGNED_BYTE, buffer_.data());

  gl->glBindTexture(GL_TEXTURE_2D, 0);
}