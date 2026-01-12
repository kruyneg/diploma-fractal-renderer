#include "app/fractal_app.h"

#include "app/ui/fractal_window.h"
#include "render/cpu/cpu_renderer.h"

#ifdef HAVE_CUDA
#include "render/cuda/cuda_renderer.h"
#include "render/cuda/utils.h"
#endif

FractalApp::FractalApp() {
#ifdef HAVE_CUDA
  if (IsCUDASupported()) {
    renderer_ = std::make_unique<render::CUDARenderer>();
  } else {
    renderer_ = std::make_unique<render::CPURenderer>();
  }
#else
  renderer_ = std::make_unique<render::CPURenderer>();
#endif

  renderer_->SetSettingsProvider(&settings_);
  main_window_ = std::make_unique<ui::FractalWindow>(this);
}

FractalApp::~FractalApp() = default;

void FractalApp::Run() { main_window_->show(); }

const render::Renderer* FractalApp::renderer() const { return renderer_.get(); }
render::Renderer* FractalApp::renderer() { return renderer_.get(); }

const SettingsManager& FractalApp::settings() const { return settings_; }
SettingsManager& FractalApp::settings() { return settings_; }
