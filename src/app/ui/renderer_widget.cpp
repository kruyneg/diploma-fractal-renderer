#include "app/ui/renderer_widget.h"

#include <QResizeEvent>

#include "app/ui/fractal_window.h"

namespace ui {

RendererWidget::RendererWidget(FractalWindow* parent /* = nullptr */,
                               render::Renderer* renderer /* = nullptr */)
    : QOpenGLWidget(parent), renderer_(renderer) {
  setFocusPolicy(Qt::StrongFocus);
}

void RendererWidget::initializeGL() {
  initializeOpenGLFunctions();

  glDisable(GL_DEPTH_TEST);
  glDisable(GL_CULL_FACE);

  texture_.Init();
  texture_.Resize(100, 100);

  renderer_->Init(texture_.texture());

  InitShader();
  InitQuad();
}

void RendererWidget::InitShader() {
  const char* vs = R"(
    #version 330 core
    layout (location = 0) in vec2 inPos;
    layout (location = 1) in vec2 inUV;
    out vec2 uv;
    void main() {
      uv = inUV;
      gl_Position = vec4(inPos, 0.0, 1.0);
    }
  )";

  const char* fs = R"(
    #version 330 core
    in vec2 uv;
    out vec4 fragColor;
    uniform sampler2D uTexture;
    void main() {
      fragColor = texture(uTexture, uv);
    }
  )";

  shader_.addShaderFromSourceCode(QOpenGLShader::Vertex, vs);
  shader_.addShaderFromSourceCode(QOpenGLShader::Fragment, fs);
  shader_.link();
}

void RendererWidget::InitQuad() {
  const float vertices[] = {
      // pos     // uv
      -1.0f, -1.0f, 0.0f, 0.0f, 1.0f, -1.0f, 1.0f, 0.0f,
      -1.0f, 1.0f,  0.0f, 1.0f, 1.0f, 1.0f,  1.0f, 1.0f,
  };

  glGenVertexArrays(1, &vao_);
  glGenBuffers(1, &vbo_);

  glBindVertexArray(vao_);
  glBindBuffer(GL_ARRAY_BUFFER, vbo_);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);

  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float),
                        (void*)(2 * sizeof(float)));

  glBindVertexArray(0);
}

void RendererWidget::resizeGL(int w, int h) {
  if (w < 0 || h < 0) {
    return;
  }

  texture_.Resize(w, h);
  if (renderer_) {
    renderer_->Resize(w, h);
  }
  glViewport(0, 0, w, h);
}

void RendererWidget::paintGL() {
  if (renderer_) {
    renderer_->Render();
  }
  DrawTexture();
}

void RendererWidget::resizeEvent(QResizeEvent* event) {
  QOpenGLWidget::resizeEvent(event);

  const auto size = event->size();
  emit ViewResized(size.width(), size.height());
}

void RendererWidget::DrawTexture() {
  shader_.bind();

  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, texture_.texture());
  shader_.setUniformValue("uTexture", 0);

  glBindVertexArray(vao_);
  glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
  glBindVertexArray(0);

  shader_.release();
}

}  // namespace ui
