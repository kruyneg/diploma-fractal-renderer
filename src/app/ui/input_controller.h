#pragma once

#include <QKeyEvent>
#include <QObject>


class SettingsManager;

namespace ui {

class InputController : QObject {
  Q_OBJECT
 public:
  explicit InputController(QObject* parent,
                           SettingsManager* settings = nullptr);

  void HandleKeyPress(QKeyEvent* event);

 private:
  SettingsManager* settings_manager_ = nullptr;
};

}  // namespace ui