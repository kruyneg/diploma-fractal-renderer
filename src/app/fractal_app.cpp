#include "app/fractal_app.h"

#include "render/cpu/cpu_renderer.h"
#include "ui/fractal_window.h"

FractalApp::FractalApp() : renderer_(std::make_unique<CPURenderer>()) {
  main_window_ = std::make_unique<ui::FractalWindow>(renderer_.get());
}

FractalApp::~FractalApp() = default;

void FractalApp::Run() { main_window_->show(); }
