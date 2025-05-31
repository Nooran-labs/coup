// coup_gui_main.cpp
#include <QApplication>
#include "MainWindow.hpp"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    coup_gui::MainWindow window;
    window.show();
    return app.exec();
}

