#include "render/cpu/cpu_renderer.h"

#include "QOpenGLFunctions"
#include "render/fractals.h"

namespace render {

CPURenderer::CPURenderer() = default;

void CPURenderer::Init(uint32_t target_tex_id) { target_ = target_tex_id; }

void CPURenderer::Resize(uint32_t w, uint32_t h) {
  width_ = w;
  height_ = h;

  buffer_.resize(w * h);
}

void CPURenderer::Render() {
  if (target_ == 0 || width_ == 0 || height_ == 0) {
    return;
  }

  const auto settings = settings_->GetSettings();

  const double scale_x =
      (settings.view.max_x - settings.view.min_x) / static_cast<double>(width_);
  const double scale_y = (settings.view.max_y - settings.view.min_y) /
                         static_cast<double>(height_);

  for (uint32_t y = 0; y < height_; ++y) {
    double fy = settings.view.max_y - y * scale_y;

    for (uint32_t x = 0; x < width_; ++x) {
      double fx = settings.view.min_x + x * scale_x;

      int iteration;
      if (settings.fractal.type == FractalType::kMandelbrot) {
        iteration =
            MandelbrotIterations(fx, fy, settings.fractal.max_iterations);
      } else if (settings.fractal.type == FractalType::kJulia) {
        iteration = JuliaIterations(fx, fy, settings.fractal.max_iterations,
                                    settings.fractal.julia.c_re,
                                    settings.fractal.julia.c_im);
      }
      Color& c = buffer_[y * width_ + x];

      c = ColorFromIter(iteration, settings.fractal.max_iterations);
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

void CPURenderer::SetSettingsProvider(SettingsProvider* settings) {
  settings_ = settings;
}

}  // namespace render
