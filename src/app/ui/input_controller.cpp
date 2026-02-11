#include "app/ui/input_controller.h"

#include <QKeyEvent>

#include "app/settings_manager.h"

namespace {

constexpr float kMoveSpeed = 1.0f;
constexpr float kZoomSpeed = 1.2;
constexpr float kMouseSensitivity = 0.002;

}  // namespace

namespace ui {

InputController::InputController(QObject* parent,
                                 SettingsManager* settings /* = nullptr */)
    : QObject(parent), settings_manager_(settings) {}

void InputController::HandleKeyPress(QKeyEvent* event) {
  if (!event->isAutoRepeat()) {
    pressed_keys_.insert(event->key());
  }
}

void InputController::HandleKeyRelease(QKeyEvent* event) {
  if (!event->isAutoRepeat()) {
    pressed_keys_.remove(event->key());
  }
}

void InputController::MouseMove(int dx, int dy) {
  const float yaw = -dx * kMouseSensitivity;
  const float pitch = -dy * kMouseSensitivity;

  if (yaw != 0.0f || pitch != 0.0f) {
    settings_manager_->RotateCamera(yaw, pitch);
  }
}

void InputController::Update(double dt) {
  if (!settings_manager_) {
    return;
  }

  float dx = 0.0f;
  float dy = 0.0f;
  float dz = 0.0f;

  if (pressed_keys_.contains(Qt::Key_A)) {
    dx -= kMoveSpeed * dt;
  }
  if (pressed_keys_.contains(Qt::Key_D)) {
    dx += kMoveSpeed * dt;
  }
  if (pressed_keys_.contains(Qt::Key_S)) {
    dy -= kMoveSpeed * dt;
  }
  if (pressed_keys_.contains(Qt::Key_W)) {
    dy += kMoveSpeed * dt;
  }
  if (pressed_keys_.contains(Qt::Key_Q)) {
    dz -= kMoveSpeed * dt;
  }
  if (pressed_keys_.contains(Qt::Key_E)) {
    dz += kMoveSpeed * dt;
  }

  if (dx != 0.0f || dy != 0.0f || dz != 0.0f) {
    settings_manager_->Move(dx, dy, dz);
  }

  float zoom = 0.0f;

  if (pressed_keys_.contains(Qt::Key_Plus) ||
      pressed_keys_.contains(Qt::Key_Equal)) {
    zoom += kZoomSpeed * dt;
  }
  if (pressed_keys_.contains(Qt::Key_Minus) ||
      pressed_keys_.contains(Qt::Key_Underscore)) {
    zoom -= kZoomSpeed * dt;
  }

  if (zoom != 0.0f) {
    settings_manager_->Zoom(std::exp(zoom));
  }

  if (pressed_keys_.contains(Qt::Key_1)) {
    settings_manager_->SetFractalType(0);
  } else if (pressed_keys_.contains(Qt::Key_2)) {
    settings_manager_->SetFractalType(1);
  } else if (pressed_keys_.contains(Qt::Key_3)) {
    settings_manager_->SetFractalType(2);
  } else if (pressed_keys_.contains(Qt::Key_4)) {
    settings_manager_->SetFractalType(3);
  } else if (pressed_keys_.contains(Qt::Key_5)) {
    settings_manager_->SetFractalType(4);
  }
}

}  // namespace ui