#include "app/ui/input_controller.h"

#include "app/settings_manager.h"

namespace {

constexpr double kDefaultMoveStep = 0.05;
constexpr double kDefaultZoomStep = 0.1;

}  // namespace

namespace ui {

InputController::InputController(QObject* parent,
                                 SettingsManager* settings /* = nullptr */)
    : QObject(parent), settings_manager_(settings) {}

void InputController::HandleKeyPress(QKeyEvent* event) {
  if (!settings_manager_) return;

  switch (event->key()) {
    case Qt::Key_Plus:
    case Qt::Key_Equal:
      settings_manager_->Zoom(1.0 + kDefaultZoomStep);
      break;
    case Qt::Key_Minus:
    case Qt::Key_Underscore:
      settings_manager_->Zoom(1.0 - kDefaultZoomStep);
      break;
    case Qt::Key_Left:
      settings_manager_->Move(-kDefaultMoveStep, 0.0);
      break;
    case Qt::Key_Right:
      settings_manager_->Move(kDefaultMoveStep, 0.0);
      break;
    case Qt::Key_Up:
      settings_manager_->Move(0.0, kDefaultMoveStep);
      break;
    case Qt::Key_Down:
      settings_manager_->Move(0.0, -kDefaultMoveStep);
      break;
    default:
      break;
  }
}

}  // namespace ui