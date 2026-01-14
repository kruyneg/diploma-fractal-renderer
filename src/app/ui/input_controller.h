#pragma once

#include <QObject>
#include <QSet>

class QKeyEvent;
class SettingsManager;

namespace ui {

class InputController : QObject {
  Q_OBJECT
 public:
  explicit InputController(QObject* parent,
                           SettingsManager* settings = nullptr);

  void HandleKeyPress(QKeyEvent* event);
  void HandleKeyRelease(QKeyEvent* event);

  void Update(double delta_seconds);

 private:
  SettingsManager* settings_manager_ = nullptr;
  QSet<int> pressed_keys_;
};

}  // namespace ui