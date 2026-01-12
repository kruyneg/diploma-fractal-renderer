#pragma once

#include <cstdint>

#include "render/settings_provider.h"

namespace render {

class Renderer {
 public:
  virtual void Init(uint32_t target_tex_id) = 0;
  virtual void Resize(uint32_t w, uint32_t h) = 0;
  virtual void Render() = 0;

  virtual void SetSettingsProvider(SettingsProvider* settings) = 0;
};

}  // namespace render