#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLineEdit>

#include "common.h"

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    void test();
    void test2();

private Q_SLOTS:
    void clear();
    void solve();

private:
    Ui::MainWindow *ui;
    QLineEdit *boxes[ENTRIES][ENTRIES];
};

#endif // MAINWINDOW_H
