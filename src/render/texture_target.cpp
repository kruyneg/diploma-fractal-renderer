#include "render/texture_target.h"

TextureTarget::TextureTarget() = default;
TextureTarget::~TextureTarget() = default;

TextureTarget::TextureTarget(TextureTarget&& other) noexcept {
  *this = std::move(other);
}

TextureTarget& TextureTarget::operator=(TextureTarget&& other) noexcept {
  if (this != &other) {
    Destroy();

    texture_id_ = other.texture_id_;
    width_ = other.width_;
    height_ = other.height_;

    other.texture_id_ = 0;
    other.width_ = 0;
    other.height_ = 0;
  }
  return *this;
}

void TextureTarget::Init() {
  if (texture_id_ != 0) {
    throw std::runtime_error("TextureTarget: double init");
  }

  glGenTextures(1, &texture_id_);
  glBindTexture(GL_TEXTURE_2D, texture_id_);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

  glBindTexture(GL_TEXTURE_2D, 0);
}

void TextureTarget::Resize(uint32_t width, uint32_t height) {
  if (texture_id_ == 0) {
    throw std::runtime_error(
        "TextureTarget: resizing an uninitialized texture");
  }
  if (width <= 0 || height <= 0) {
    throw std::invalid_argument("TextureTarget: zero or negative size");
  }

  width_ = width;
  height_ = height;

  glBindTexture(GL_TEXTURE_2D, texture_id_);

  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width_, height_, 0, GL_RGBA,
               GL_UNSIGNED_BYTE, nullptr);

  glBindTexture(GL_TEXTURE_2D, 0);
}

void TextureTarget::Destroy() {
  if (texture_id_ != 0) {
    glDeleteTextures(1, &texture_id_);
    texture_id_ = 0;
  }

  width_ = 0;
  height_ = 0;
}

GLuint TextureTarget::texture() const { return texture_id_; }
uint32_t TextureTarget::width() const { return width_; }
uint32_t TextureTarget::height() const { return height_; }
