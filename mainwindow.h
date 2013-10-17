#pragma once

#include <QtWidgets/QMainWindow>
#include <QtWidgets/QLineEdit>
#include <QtCore/QVector>

#include "common.h"

namespace sudoku {

// Single-Widget GUI for the sudoku solver. There's one QLineEdit box per soduku field.
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow( QWidget * const parent );

private:
    void test();

private Q_SLOTS:
    void clear();
    void solve();

private:
    static const int LINEEDIT_PIXEL_WIDTH = 20;

    QVector< QVector< QLineEdit * > > boxes; // 2-dimensional data structure on the QLineEdits
};

} // namespace sudoku
