#pragma once

#include <QOpenGLFunctions_3_3_Core>
#include <QOpenGLShaderProgram>
#include <QtOpenGLWidgets/QOpenGLWidget>

#include "app/ui/texture_target.h"
#include "render/renderer.h"

namespace ui {

class FractalWindow;
class InputController;

class RendererWidget : public QOpenGLWidget,
                       protected QOpenGLFunctions_3_3_Core {
  Q_OBJECT
 public:
  RendererWidget(FractalWindow* parent = nullptr,
                 render::Renderer* renderer = nullptr);

  void UpdateSettings(double delta_seconds);

 signals:
  void ViewResized(uint32_t w, uint32_t h);

 protected:
  void initializeGL() override;
  void resizeGL(int w, int h) override;
  void paintGL() override;

  void resizeEvent(QResizeEvent* event) override;
  void focusInEvent(QFocusEvent* event) override;
  void focusOutEvent(QFocusEvent* event) override;
  void keyPressEvent(QKeyEvent* event) override;
  void keyReleaseEvent(QKeyEvent* event) override;
  void mousePressEvent(QMouseEvent* event) override;
  void mouseMoveEvent(QMouseEvent* event) override;

 private:
  void InitShader();
  void InitQuad();
  void DrawTexture();

  QPoint GetCenter() const;

  InputController* input_controller_;
  bool mouse_locked_ = false;
  bool ignore_next_mouse_event_ = false;

  render::Renderer* renderer_;
  TextureTarget texture_;

  QOpenGLShaderProgram shader_;
  GLuint vao_ = 0;
  GLuint vbo_ = 0;
};

}  // namespace ui
