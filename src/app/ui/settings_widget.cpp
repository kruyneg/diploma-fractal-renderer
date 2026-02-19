#include "app/ui/settings_widget.h"

#include <QComboBox>
#include <QFormLayout>
#include <QLabel>
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

class MandelbulbSettingsWidget final : public QWidget {
  Q_OBJECT
 public:
  explicit MandelbulbSettingsWidget(QWidget* parent, SettingsManager* settings)
      : QWidget(parent), settings_(settings) {
    auto* layout = new QFormLayout(this);

    power_ = new QDoubleSpinBox(this);
    power_->setRange(1.0, 15.0);
    power_->setSingleStep(0.1);
    boilout_ = new QDoubleSpinBox(this);
    boilout_->setRange(0, 10.0);
    boilout_->setSingleStep(0.1);

    layout->addRow("Power", power_);
    layout->addRow("Boilout", boilout_);

    connect(power_, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this,
            &MandelbulbSettingsWidget::OnParamsChanged);
    connect(boilout_, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            this, &MandelbulbSettingsWidget::OnParamsChanged);
  }

  void SyncFromSettings(const render::MandelbulbParams& params) {
    power_->setValue(params.power);
    boilout_->setValue(params.boilout);
  }

 private slots:
  void OnParamsChanged() {
    if (!settings_) return;
    render::MandelbulbParams params;
    params.power = power_->value();
    params.boilout = boilout_->value();

    settings_->SetMandelbulbParams(params);
  }

 private:
  QDoubleSpinBox* power_;
  QDoubleSpinBox* boilout_;

  SettingsManager* settings_;
};

class MandelboxSettingsWidget final : public QWidget {
  Q_OBJECT
 public:
  explicit MandelboxSettingsWidget(QWidget* parent, SettingsManager* settings)
      : QWidget(parent), settings_(settings) {
    auto* layout = new QFormLayout(this);

    min_radius_ = new QDoubleSpinBox(this);
    min_radius_->setRange(0.0, 2.0);
    min_radius_->setSingleStep(0.01);
    fixed_radius_ = new QDoubleSpinBox(this);
    fixed_radius_->setRange(0, 10.0);
    fixed_radius_->setSingleStep(1.0);
    scale_ = new QDoubleSpinBox(this);
    scale_->setRange(-5.0, 5.0);
    scale_->setSingleStep(0.1);

    layout->addRow("Min radius", min_radius_);
    layout->addRow("Fixed radius", fixed_radius_);
    layout->addRow("Scale", scale_);

    connect(min_radius_, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            this, &MandelboxSettingsWidget::OnParamsChanged);
    connect(fixed_radius_, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            this, &MandelboxSettingsWidget::OnParamsChanged);
    connect(scale_, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this,
            &MandelboxSettingsWidget::OnParamsChanged);
  }

  void SyncFromSettings(const render::MandelboxParams& params) {
    min_radius_->setValue(params.min_radius);
    fixed_radius_->setValue(params.fixed_radius);
    scale_->setValue(params.scale);
  }

 private slots:
  void OnParamsChanged() {
    if (!settings_) return;
    render::MandelboxParams params;
    params.min_radius = min_radius_->value();
    params.fixed_radius = fixed_radius_->value();
    params.scale = scale_->value();

    settings_->SetMandelboxParams(params);
  }

 private:
  QDoubleSpinBox* min_radius_;
  QDoubleSpinBox* fixed_radius_;
  QDoubleSpinBox* scale_;

  SettingsManager* settings_;
};

class JuliabulbSettingsWidget final : public QWidget {
  Q_OBJECT
 public:
  explicit JuliabulbSettingsWidget(QWidget* parent, SettingsManager* settings)
      : QWidget(parent), settings_(settings) {
    auto* layout = new QFormLayout(this);

    power_ = new QDoubleSpinBox(this);
    power_->setRange(1.0, 15.0);
    power_->setSingleStep(0.1);
    c_x_ = new QDoubleSpinBox(this);
    c_x_->setRange(-1.0, 1.0);
    c_x_->setSingleStep(0.1);
    c_y_ = new QDoubleSpinBox(this);
    c_y_->setRange(-1.0, 1.0);
    c_y_->setSingleStep(0.1);
    c_z_ = new QDoubleSpinBox(this);
    c_z_->setRange(-1.0, 1.0);
    c_z_->setSingleStep(0.1);

    layout->addRow("Power", power_);
    layout->addRow("C.x", c_x_);
    layout->addRow("C.y", c_y_);
    layout->addRow("C.z", c_z_);

    connect(power_, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this,
            &JuliabulbSettingsWidget::OnParamsChanged);
    connect(c_x_, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this,
            &JuliabulbSettingsWidget::OnParamsChanged);
    connect(c_y_, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this,
            &JuliabulbSettingsWidget::OnParamsChanged);
    connect(c_z_, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this,
            &JuliabulbSettingsWidget::OnParamsChanged);
  }

  void SyncFromSettings(const render::JuliabulbParams& params) {
    power_->setValue(params.power);
    c_x_->setValue(params.c.x);
    c_y_->setValue(params.c.y);
    c_z_->setValue(params.c.z);
  }

 private slots:
  void OnParamsChanged() {
    if (!settings_) return;
    render::JuliabulbParams params;
    params.power = power_->value();
    params.c.x = c_x_->value();
    params.c.y = c_y_->value();
    params.c.z = c_z_->value();

    settings_->SetJuliabulbParams(params);
  }

 private:
  QDoubleSpinBox* power_;
  QDoubleSpinBox* c_x_;
  QDoubleSpinBox* c_y_;
  QDoubleSpinBox* c_z_;

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
  if (auto* mandelbulb_widget = dynamic_cast<MandelbulbSettingsWidget*>(
          fractal_stack_->currentWidget())) {
    mandelbulb_widget->SyncFromSettings(settings.fractal.mandelbulb);
  }
  if (auto* mandelbox_widget = dynamic_cast<MandelboxSettingsWidget*>(
          fractal_stack_->currentWidget())) {
    mandelbox_widget->SyncFromSettings(settings.fractal.mandelbox);
  }
  if (auto* juliabulb_widget = dynamic_cast<JuliabulbSettingsWidget*>(
          fractal_stack_->currentWidget())) {
    juliabulb_widget->SyncFromSettings(settings.fractal.juliabulb);
  }

  iterations_spin_->setValue(settings.fractal.max_iterations);
}

void SettingsWidget::SetFrameStats(double ms, double fps) {
  fps_label_->setText(QString("Render Time: %1\nFPS: %2").arg(ms).arg(fps));
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
  fractal_combo_->addItem(
      "Mandelbulb", static_cast<uint8_t>(render::FractalType::kMandelbulb));
  fractal_combo_->addItem(
      "Mandelbox", static_cast<uint8_t>(render::FractalType::kMandelbox));
  fractal_combo_->addItem(
      "Juliabulb", static_cast<uint8_t>(render::FractalType::kJuliabulb));

  connect(fractal_combo_, QOverload<int>::of(&QComboBox::currentIndexChanged),
          this, &SettingsWidget::OnFractalTypeChanged);

  top_form->addRow("Fractal Type", fractal_combo_);

  iterations_spin_ = new QSpinBox(this);
  iterations_spin_->setRange(1, 256);
  connect(iterations_spin_, &QSpinBox::valueChanged, this,
          &SettingsWidget::OnIterationsChanged);

  top_form->addRow("Iterations", iterations_spin_);

  layout->addLayout(top_form);

  auto* separator = new QFrame(this);
  separator->setFrameShape(QFrame::HLine);
  layout->addWidget(separator);

  fractal_stack_ = new QStackedWidget(this);
  layout->addWidget(fractal_stack_);
  // Mandelbrot
  fractal_stack_->addWidget(new QWidget(this));
  // Julia
  fractal_stack_->addWidget(new JuliaSettingsWidget(this, settings_manager_));
  // Menger Sponge
  fractal_stack_->addWidget(new QWidget(this));
  // Mandelbulb
  fractal_stack_->addWidget(
      new MandelbulbSettingsWidget(this, settings_manager_));
  // Mandelbox
  fractal_stack_->addWidget(
      new MandelboxSettingsWidget(this, settings_manager_));
  // Juliabulb
  fractal_stack_->addWidget(
      new JuliabulbSettingsWidget(this, settings_manager_));

  layout->addStretch();

  fps_label_ = new QLabel(this);
  layout->addWidget(fps_label_);
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
