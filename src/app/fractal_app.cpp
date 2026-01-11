#include "app/fractal_app.h"

#include "render/cpu/cpu_renderer.h"
#include "ui/fractal_window.h"

#ifdef HAVE_CUDA
#include "render/cuda/cuda_renderer.h"
#include "render/cuda/utils.h"
#endif

FractalApp::FractalApp() {
#ifdef HAVE_CUDA
  if (IsCUDASupported()) {
    renderer_ = std::make_unique<CUDARenderer>();
  } else {
    renderer_ = std::make_unique<CPURenderer>();
  }
#else
  renderer_ = std::make_unique<CPURenderer>();
#endif
  main_window_ = std::make_unique<ui::FractalWindow>(renderer_.get());
}

FractalApp::~FractalApp() = default;

void FractalApp::Run() { main_window_->show(); }
