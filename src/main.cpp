#include <QApplication>
#include <QMainWindow>
#include <QLabel>

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);

    QMainWindow window;
    window.setWindowTitle("Qt Test");

    QLabel* label = new QLabel("Hello, World!");
    label->setAlignment(Qt::AlignCenter);

    window.setCentralWidget(label);
    window.resize(600, 400);
    window.show();

    return app.exec();
}
