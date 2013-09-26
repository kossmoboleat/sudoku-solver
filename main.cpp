#include <QtWidgets/QApplication>
#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    sudoku::MainWindow w(0);
    w.show();

    return a.exec();
}
