#include "app/ui/renderer_widget.h"

#include <QCursor>
#include <QResizeEvent>

#include "app/fractal_app.h"
#include "app/ui/fractal_window.h"
#include "app/ui/input_controller.h"

namespace ui {

RendererWidget::RendererWidget(FractalWindow* parent /* = nullptr */,
                               render::Renderer* renderer /* = nullptr */)
    : QOpenGLWidget(parent), renderer_(renderer) {
  input_controller_ = new InputController(this, &parent->app()->settings());

  setFocusPolicy(Qt::StrongFocus);
  setMouseTracking(true);
}

void RendererWidget::UpdateSettings(double dt) {
  input_controller_->Update(dt);
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
  frame_timer_.restart();

  if (renderer_) {
    renderer_->Render();
  }
  DrawTexture();

  const double frame_ms = frame_timer_.nsecsElapsed() * 1e-6;
  const double fps = 1000.0 / frame_ms;
  emit FrameStatsUpdated(frame_ms, fps);
}

void RendererWidget::resizeEvent(QResizeEvent* event) {
  QOpenGLWidget::resizeEvent(event);

  const auto size = event->size();
  emit ViewResized(size.width(), size.height());
}

void RendererWidget::focusInEvent(QFocusEvent*) {
  mouse_locked_ = true;

  setCursor(Qt::BlankCursor);
  grabMouse();

  QCursor::setPos(GetCenter());
  ignore_next_mouse_event_ = true;
}

void RendererWidget::focusOutEvent(QFocusEvent* event) {
  mouse_locked_ = false;

  releaseMouse();
  unsetCursor();
}

void RendererWidget::keyPressEvent(QKeyEvent* event) {
  if (event->key() == Qt::Key_Escape) {
    clearFocus();
    releaseMouse();
    return;
  }
  input_controller_->HandleKeyPress(event);
}

void RendererWidget::keyReleaseEvent(QKeyEvent* event) {
  input_controller_->HandleKeyRelease(event);
}

void RendererWidget::mousePressEvent(QMouseEvent* event) {
  setFocus(Qt::MouseFocusReason);
  grabMouse();
  QWidget::mousePressEvent(event);
}

void RendererWidget::mouseMoveEvent(QMouseEvent* event) {
  if (!mouse_locked_) {
    QOpenGLWidget::mouseMoveEvent(event);
    return;
  }

  if (ignore_next_mouse_event_) {
    ignore_next_mouse_event_ = false;
    return;
  }

  const auto center = mapFromGlobal(GetCenter());
  const auto delta = event->pos() - center;
  input_controller_->MouseMove(delta.x(), delta.y());

  QCursor::setPos(GetCenter());
  ignore_next_mouse_event_ = true;
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

QPoint RendererWidget::GetCenter() const {
  return mapToGlobal(rect().center());
}

}  // namespace ui
