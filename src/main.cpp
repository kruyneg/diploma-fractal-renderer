#include <QApplication>

#include "app/fractal_app.h"

int main(int argc, char* argv[]) {
  QApplication qt(argc, argv);

  FractalApp app;
  app.Run();

  return qt.exec();
}
