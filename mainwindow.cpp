#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <iostream>
#include <sstream>

#include <QGridLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QMessageBox>

#include <QPalette>

#include "solver.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi( this );

    // vboxLayout contains the grid of lineedits and the button
    QVBoxLayout *vboxLayout = new QVBoxLayout();

    QGridLayout *gridLayout = new QGridLayout();

    for(int i=0;i<ENTRIES; i++) {
        for(int j=0;j<ENTRIES;j++) {
            boxes[i][j] = new QLineEdit();
            boxes[i][j]->setFixedSize( 20, 20 );
            gridLayout->addWidget( boxes[i][j], i, j );
        }
    }

    vboxLayout->addLayout( gridLayout );

    QHBoxLayout *hboxLayout = new QHBoxLayout();
    vboxLayout->addLayout( hboxLayout );

    QPushButton *solveButton = new QPushButton("Solve!");
    hboxLayout->addWidget( solveButton );

    QPushButton *clearButton = new QPushButton("Clear");
    hboxLayout->addWidget( clearButton );

    centralWidget()->setLayout( vboxLayout );

    connect( solveButton, SIGNAL(clicked()), this, SLOT(solve()));
    connect( clearButton, SIGNAL(clicked()), this, SLOT(clear()));

    test2();
}

void MainWindow::test2()
{
    boxes[0][0]->setText("4");
    boxes[0][4]->setText("1");

    boxes[1][3]->setText("3");
    boxes[1][5]->setText("9");
    boxes[1][7]->setText("4");

    boxes[2][1]->setText("7");
    boxes[2][5]->setText("5");
    boxes[2][8]->setText("9");

    boxes[3][4]->setText("6");
    boxes[3][7]->setText("2");
    boxes[3][8]->setText("1");

    boxes[4][2]->setText("4");
    boxes[4][4]->setText("7");
    boxes[4][6]->setText("6");

    boxes[5][0]->setText("1");
    boxes[5][1]->setText("9");
    boxes[5][4]->setText("5");

    boxes[6][0]->setText("9");
    boxes[6][3]->setText("4");
    boxes[6][7]->setText("7");

    boxes[7][1]->setText("3");
    boxes[7][3]->setText("6");
    boxes[7][5]->setText("8");

    boxes[8][4]->setText("3");
    boxes[8][8]->setText("6");
}

void MainWindow::test()
{
    boxes[0][3]->setText("1");
    boxes[0][5]->setText("5");

    boxes[1][0]->setText("1");
    boxes[1][1]->setText("4");
    boxes[1][6]->setText("6");
    boxes[1][7]->setText("7");

    boxes[2][1]->setText("8");
    boxes[2][5]->setText("2");
    boxes[2][6]->setText("4");

    boxes[3][1]->setText("6");
    boxes[3][2]->setText("3");
    boxes[3][4]->setText("7");
    boxes[3][7]->setText("1");

    boxes[4][0]->setText("9");
    boxes[4][8]->setText("3");

    boxes[5][1]->setText("1");
    boxes[5][4]->setText("9");
    boxes[5][6]->setText("5");
    boxes[5][7]->setText("2");

    boxes[6][2]->setText("7");
    boxes[6][3]->setText("2");
    boxes[6][7]->setText("8");

    boxes[7][1]->setText("2");
    boxes[7][2]->setText("6");
    boxes[7][7]->setText("3");
    boxes[7][8]->setText("5");

    boxes[8][3]->setText("4");
    boxes[8][5]->setText("9");
}

void MainWindow::clear()
{
    for(int i=0;i<ENTRIES;i++) {
        for(int j=0;j<ENTRIES;j++) {
            QLineEdit* cell = boxes[i][j];
            cell->clear();

            // make cell background white again
            QPalette cell_pallete = cell->palette();
            cell_pallete.setColor(QPalette::Base, QColor(255,255,255));
            cell->setPalette(cell_pallete);
        }
    }
}

void MainWindow::solve()
{
    int fixed_numbers[ENTRIES][ENTRIES];

    // convert content of boxes to two-dimensional integer array
    bool error=false;
    for(int i=0;i<ENTRIES;i++) {
        for(int j=0;j<ENTRIES;j++) {
            fixed_numbers[i][j]=0;
            QString const text( boxes[i][j]->text() );
            if( !text.isEmpty() ) {
                bool conversion_success = false;
                int const converted_number = text.toInt(&conversion_success);
                if( conversion_success && converted_number>0 && converted_number<=ENTRIES ) {
                    fixed_numbers[i][j] = converted_number;
                } else {
                    error = true;
                    std::ostringstream oss;
                    oss << text.toStdString() << " is not a valid input number at " << i << "," << j << std::endl;
                    QMessageBox::critical(centralWidget(), QString( "Error" ),
                                          QString::fromStdString( oss.str() ),
                                          QMessageBox::Ok);
                }
            }
        }
    }

    if( error ) {
        return;
    }

    // Try solving the Sudoku
    Solver *solver = new Solver(fixed_numbers);
    bool solved = solver->solve();
    if( !solved ) {
        QMessageBox::critical(centralWidget(), QString( "Error" ),
                              QString( "This Sudoku cannot be solved!"),
                              QMessageBox::Ok);
    } else {
        QVector< QVector< int > > solved_numbers = solver->getSolution();

        // enter solution in missing boxes
        for(int i=0;i<ENTRIES;i++) {
            for(int j=0;j<ENTRIES;j++) {
                QLineEdit *cell = boxes[i][j];

                if(cell->text().isEmpty()) {
                    QString solved_value = QString::number(solved_numbers[i][j]);
                    cell->setText( solved_value );

                    //make cell green
                    QPalette cell_pallete = cell->palette();
                    cell_pallete.setColor(QPalette::Base, QColor(0,255,0));
                    cell->setPalette(cell_pallete);
                }
            }
        }
    }

    delete solver;
}

MainWindow::~MainWindow()
{
    delete ui;
}
