#pragma once

#include <vector>

#include "render/renderer.h"
#include "render/utils.h"

class CPURenderer : public Renderer {
 public:
  CPURenderer();

  void Resize(uint32_t w, uint32_t h) override;
  void Render(TextureTarget* target) override;

 private:
  void UploadBufferToTexture(TextureTarget* target) const;

  uint32_t width_ = 0;
  uint32_t height_ = 0;

  std::vector<Color> buffer_;
};
