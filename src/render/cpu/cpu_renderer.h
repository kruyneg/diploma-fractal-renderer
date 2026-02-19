#pragma once

#include <vector>

#include "render/common/types.h"
#include "render/renderer.h"

namespace render {

class CPURenderer : public Renderer {
 public:
  CPURenderer();

  void Init(uint32_t target_tex_id) override;
  void Resize(uint32_t w, uint32_t h) override;
  void Render() override;
  void SetSettingsProvider(SettingsProvider* settings) override;

 private:
  void Render2D(const RenderSettings& settings);
  void Render3D(const RenderSettings& settings);
  void UploadBufferToTarget() const;

  uint32_t width_ = 0;
  uint32_t height_ = 0;

  SettingsProvider* settings_ = nullptr;

  uint32_t target_ = 0;
  std::vector<Color> buffer_;
};

}  // namespace render
