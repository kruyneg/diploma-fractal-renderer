#pragma once

#include <QOpenGLFunctions_3_3_Core>
#include <QOpenGLShaderProgram>
#include <QtOpenGLWidgets/QOpenGLWidget>

#include "app/ui/texture_target.h"
#include "render/renderer.h"

namespace ui {

class FractalWindow;

class RendererWidget : public QOpenGLWidget,
                       protected QOpenGLFunctions_3_3_Core {
  Q_OBJECT
 public:
  RendererWidget(FractalWindow* parent = nullptr,
                 render::Renderer* renderer = nullptr);

 protected:
  void initializeGL() override;
  void resizeGL(int w, int h) override;
  void paintGL() override;

 private:
  void InitShader();
  void InitQuad();

  void DrawTexture();

  render::Renderer* renderer_;
  TextureTarget texture_;

  QOpenGLShaderProgram shader_;
  GLuint vao_ = 0;
  GLuint vbo_ = 0;
};

}  // namespace ui
