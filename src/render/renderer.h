#pragma once

#include <cstdint>

class TextureTarget;

class Renderer {
 public:
  virtual void Resize(uint32_t w, uint32_t h) = 0;
  virtual void Render(TextureTarget* target) = 0;
};
