#include "app/ui/input_controller.h"

#include <QKeyEvent>

#include "app/settings_manager.h"

namespace {

constexpr double kMoveSpeed = 1.0;
constexpr double kZoomSpeed = 1.2;

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

void InputController::Update(double dt) {
  if (!settings_manager_) {
    return;
  }

  double dx = 0.0;
  double dy = 0.0;
  double dz = 0.0;

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

  if (dx != 0.0 || dy != 0.0 || dz != 0.0) {
    settings_manager_->Move(dx, dy, dz);
  }

  double zoom = 0.0;

  if (pressed_keys_.contains(Qt::Key_Plus) ||
      pressed_keys_.contains(Qt::Key_Equal)) {
    zoom += kZoomSpeed * dt;
  }
  if (pressed_keys_.contains(Qt::Key_Minus) ||
      pressed_keys_.contains(Qt::Key_Underscore)) {
    zoom -= kZoomSpeed * dt;
  }

  if (zoom != 0.0) {
    settings_manager_->Zoom(std::exp(zoom));
  }

  if (pressed_keys_.contains(Qt::Key_1)) {
    settings_manager_->SetFractalType(0);
  } else if (pressed_keys_.contains(Qt::Key_2)) {
    settings_manager_->SetFractalType(1);
  } else if (pressed_keys_.contains(Qt::Key_3)) {
    settings_manager_->SetFractalType(2);
  }
}

}  // namespace ui