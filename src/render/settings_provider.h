#pragma once

#include <cstdint>

namespace render {

struct RenderSettings {
  uint32_t max_iter;

  double min_x;
  double min_y;
  double max_x;
  double max_y;
};

class SettingsProvider {
 public:
  virtual RenderSettings GetSettings() = 0;
};

}  // namespace render
