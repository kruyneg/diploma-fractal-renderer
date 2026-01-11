#pragma once

#include <cuda_runtime.h>

#include "render/renderer.h"

class CUDARenderer : public Renderer {
 public:
  CUDARenderer();
  ~CUDARenderer();

  void Init(uint32_t target_tex_id) override;
  void Resize(uint32_t w, uint32_t h) override;
  void Render() override;

 private:
  uint32_t width_ = 0;
  uint32_t height_ = 0;

  // cudaGraphicsResource* cuda_resource_ = nullptr;
  uint32_t gl_tex_id_ = 0;

  bool initialized_ = false;
};
