#pragma once

#include <QOpenGLFunctions_3_3_Core>
#include <QOpenGLShaderProgram>
#include <QtOpenGLWidgets/QOpenGLWidget>

#include "render/renderer.h"
#include "render/texture_target.h"


namespace ui {

class FractalWindow;

class RendererWidget : public QOpenGLWidget,
                       protected QOpenGLFunctions_3_3_Core {
  Q_OBJECT
 public:
  RendererWidget(FractalWindow* parent = nullptr, Renderer* renderer = nullptr);

 protected:
  void initializeGL() override;
  void resizeGL(int w, int h) override;
  void paintGL() override;

 private:
  void InitShader();
  void InitQuad();

  void DrawTexture();

  Renderer* renderer_;
  TextureTarget texture_;

  QOpenGLShaderProgram shader_;
  GLuint vao_ = 0;
  GLuint vbo_ = 0;
};

}  // namespace ui
