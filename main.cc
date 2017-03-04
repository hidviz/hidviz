#include <QApplication>
#include <QMainWindow>
#include <QVBoxLayout>
#include "Window.hh"

int main(int argc, char** argv) {
    QApplication app(argc, argv);

    Window o;

    return app.exec();
}