#include "Window.hh"

#include <QApplication>

int main(int argc, char** argv) {
    QApplication app(argc, argv);

    Window o;

    return app.exec();
}