#include "app/ui/settings_widget.h"

#include <QComboBox>
#include <QFormLayout>
#include <QSpinBox>
#include <QStackedWidget>
#include <QVBoxLayout>

#include "app/settings_manager.h"
#include "render/settings_provider.h"

namespace {

class JuliaSettingsWidget final : public QWidget {
  Q_OBJECT
 public:
  explicit JuliaSettingsWidget(QWidget* parent, SettingsManager* settings)
      : QWidget(parent), settings_(settings) {
    auto* layout = new QFormLayout(this);

    real_ = new QDoubleSpinBox(this);
    real_->setRange(-10.0, 10.0);
    real_->setSingleStep(0.01);
    real_->setDecimals(2);
    imagine_ = new QDoubleSpinBox(this);
    imagine_->setRange(0, 1.0);
    imagine_->setSingleStep(0.001);
    imagine_->setDecimals(3);

    layout->addRow("c.real", real_);
    layout->addRow("c.imagine", imagine_);

    connect(real_, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this,
            &JuliaSettingsWidget::OnParamsChanged);
    connect(imagine_, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            this, &JuliaSettingsWidget::OnParamsChanged);
  }

  void SyncFromSettings(const render::JuliaParams& params) {
    real_->setValue(params.c_re);
    imagine_->setValue(params.c_im);
  }

 private slots:
  void OnParamsChanged() {
    if (!settings_) return;
    render::JuliaParams params;
    params.c_re = real_->value();
    params.c_im = imagine_->value();

    settings_->SetJuliaParams(params);
  }

 private:
  QDoubleSpinBox* real_;
  QDoubleSpinBox* imagine_;

  SettingsManager* settings_;
};

}  // namespace

namespace ui {

SettingsWidget::SettingsWidget(QWidget* parent, SettingsManager* settings)
    : QWidget(parent), settings_manager_(settings) {
  BuildUI();
}

void SettingsWidget::SyncWithSettings() {
  const auto settings = settings_manager_->GetSettings();

  fractal_combo_->setCurrentIndex(static_cast<int>(settings.fractal.type));
  fractal_stack_->setCurrentIndex(static_cast<int>(settings.fractal.type));

  if (auto* julia_widget =
          dynamic_cast<JuliaSettingsWidget*>(fractal_stack_->currentWidget())) {
    julia_widget->SyncFromSettings(settings.fractal.julia);
  }

  iterations_spin_->setValue(settings.fractal.max_iterations);
}

void SettingsWidget::BuildUI() {
  auto* layout = new QVBoxLayout(this);

  auto* top_form = new QFormLayout(this);

  fractal_combo_ = new QComboBox(this);
  fractal_combo_->addItem(
      "Mandelbrot", static_cast<uint8_t>(render::FractalType::kMandelbrot));
  fractal_combo_->addItem("Julia",
                          static_cast<uint8_t>(render::FractalType::kJulia));
  fractal_combo_->addItem(
      "Menger Sponge",
      static_cast<uint8_t>(render::FractalType::kMengerSponge));

  connect(fractal_combo_, QOverload<int>::of(&QComboBox::currentIndexChanged),
          this, &SettingsWidget::OnFractalTypeChanged);

  top_form->addRow("Fractal Type", fractal_combo_);

  iterations_spin_ = new QSpinBox(this);
  iterations_spin_->setRange(1, 256);
  connect(iterations_spin_, &QSpinBox::valueChanged, this,
          &SettingsWidget::OnIterationsChanged);

  top_form->addRow("Power", iterations_spin_);

  layout->addLayout(top_form);

  auto* separator = new QFrame(this);
  separator->setFrameShape(QFrame::HLine);
  layout->addWidget(separator);

  fractal_stack_ = new QStackedWidget(this);
  layout->addWidget(fractal_stack_);

  fractal_stack_->addWidget(new QWidget(this));

  fractal_stack_->addWidget(new JuliaSettingsWidget(this, settings_manager_));

  fractal_stack_->addWidget(new QWidget(this));

  layout->addStretch(1);
}

void SettingsWidget::OnFractalTypeChanged(int index) {
  auto type = static_cast<uint8_t>(fractal_combo_->itemData(index).toInt());

  fractal_stack_->setCurrentIndex(type);
  settings_manager_->SetFractalType(type);
}

void SettingsWidget::OnIterationsChanged(uint32_t value) {
  settings_manager_->SetMaxIterations(static_cast<uint32_t>(value));
}

}  // namespace ui

#include "settings_widget.moc"
