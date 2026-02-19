#include "render/cpu/cpu_renderer.h"

#include "QOpenGLFunctions"
#include "render/common/coloring.h"
#include "render/common/fractals.h"
#include "render/common/utils.h"

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
  if (Is2DFractal(settings.fractal.type)) {
    Render2D(settings);
  } else {
    Render3D(settings);
  }

  UploadBufferToTarget();
}

void CPURenderer::Render2D(const RenderSettings& settings) {
  for (uint32_t y = 0; y < height_; ++y) {
    for (uint32_t x = 0; x < width_; ++x) {
      const auto pos = PixelToPosition(x, y, width_, height_, settings.camera);

      int iteration;
      if (settings.fractal.type == FractalType::kMandelbrot) {
        iteration =
            MandelbrotIterations(pos.x, pos.y, settings.fractal.max_iterations);
      } else if (settings.fractal.type == FractalType::kJulia) {
        iteration = JuliaIterations(
            pos.x, pos.y, settings.fractal.max_iterations,
            settings.fractal.julia.c_re, settings.fractal.julia.c_im);
      }
      Color& c = buffer_[y * width_ + x];

      c = ColorFromIter(iteration, settings.fractal.max_iterations);
    }
  }
}

void CPURenderer::Render3D(const RenderSettings& settings) {
  for (uint32_t y = 0; y < height_; ++y) {
    for (uint32_t x = 0; x < width_; ++x) {
      Color& color = buffer_[y * width_ + x];
      color = {0, 0, 0, 255};

      const auto ray = MakeRay(x, y, width_, height_, settings.camera);
      float t = 0.0f;

      for (int i = 0; i < 100; ++i) {
        const auto pos = ray.position + ray.direction * t;
        const auto distance = CalculateSignedDistance(pos, settings);

        if (distance < 0.001 * t) {
          const auto n = GetNormal(pos, settings);
          color = render::GetFractalColor(pos, n, settings.fractal);
          break;
        }
        if (distance > 2.0f) {
          color = {100, 100, 100, 255};
          break;
        }

        t += distance;
      }
    }
  }
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
