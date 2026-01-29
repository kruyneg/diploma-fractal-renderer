#pragma once

#include <QWidget>

class SettingsManager;
class QComboBox;
class QSpinBox;
class QStackedWidget;

namespace ui {

class SettingsWidget : public QWidget {
  Q_OBJECT
 public:
  SettingsWidget(QWidget* parent, SettingsManager* settings);

  void SyncWithSettings();

 private:
  void BuildUI();

  void OnFractalTypeChanged(int type);
  void OnIterationsChanged(uint32_t iterations);

  SettingsManager* settings_manager_;

  QComboBox* fractal_combo_;
  QSpinBox* iterations_spin_;
  QStackedWidget* fractal_stack_;
};

}  // namespace ui