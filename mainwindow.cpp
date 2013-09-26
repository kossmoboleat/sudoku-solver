#include "mainwindow.h"

#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QMessageBox>
#include <QtWidgets/QFrame>
#include <QtGui/QPalette>

#include "solver.h"

namespace sudoku {

MainWindow::MainWindow( QWidget * const parent )
    : QMainWindow( parent )
    , boxes( NUM_ENTRIES, QVector< QLineEdit * >(NUM_ENTRIES) )
{
    setWindowTitle( tr("Sudoku Solver") );

    // vboxLayout contains the grid of lineedits and the button
    QVBoxLayout *vboxLayout = new QVBoxLayout();

    QGridLayout *gridLayout = new QGridLayout();

    for(int i=0; i<NUM_ENTRIES; i++) {
        for(int j=0; j<NUM_ENTRIES; j++) {
            boxes[i][j] = new QLineEdit();
            boxes[i][j]->setFixedSize( MainWindow::LINEEDIT_PIXEL_WIDTH, MainWindow::LINEEDIT_PIXEL_WIDTH );
            gridLayout->addWidget( boxes[i][j], i, j );
        }
    }

    vboxLayout->addLayout( gridLayout );

    QHBoxLayout *hboxLayout = new QHBoxLayout();
    vboxLayout->addLayout( hboxLayout );

    QPushButton *solveButton = new QPushButton( tr("Solve!") );
    hboxLayout->addWidget( solveButton );

    QPushButton *clearButton = new QPushButton( tr("Clear") );
    hboxLayout->addWidget( clearButton );

    setCentralWidget( new QFrame() );

    centralWidget()->setLayout( vboxLayout );

    connect( solveButton, SIGNAL(clicked()), this, SLOT(solve()));
    connect( clearButton, SIGNAL(clicked()), this, SLOT(clear()));

    test();
}

void MainWindow::test()
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

void MainWindow::clear()
{
    for(int i=0; i<NUM_ENTRIES; i++) {
        for(int j=0; j<NUM_ENTRIES; j++) {
            QLineEdit * const cell = boxes[i][j];
            if( cell ) {
                cell->clear();

                // make cell background white again
                QPalette cell_pallete = cell->palette();
                cell_pallete.setColor( QPalette::Base, QColor( 255,255,255 ) );

                cell->setPalette( cell_pallete );
            }
        }
    }
}

void MainWindow::solve()
{
    QVector< QVector<int> > fixed_numbers( NUM_ENTRIES, QVector<int>(NUM_ENTRIES));

    // convert content of boxes to two-dimensional integer array
    for(int i=0; i<NUM_ENTRIES; i++) {
        for(int j=0; j<NUM_ENTRIES; j++) {
            fixed_numbers[i][j]=0;

            QString const text( boxes[i][j]->text() );
            if( !text.isEmpty() ) {

                bool conversion_success = false;
                int const converted_number = text.toInt( &conversion_success );

                if( conversion_success && 
                    converted_number>0 && converted_number<=NUM_ENTRIES ) {
                    fixed_numbers[i][j] = converted_number;
                } else {
                    QMessageBox::critical( centralWidget(),
                                           tr( "Error" ),
                                           tr( "%1 is not a valid input number at %2 %3" ).arg( text ).arg( i ).arg( j ),
                                           QMessageBox::Ok );

                    return;
                }
            }
        }
    }

    // Try solving the Sudoku
    Solver solver(fixed_numbers);

    if( !solver.solve() ) {
        QMessageBox::critical( centralWidget(),
                               tr( "Error" ),
                               tr( "This sudoku cannot be solved!"),
                               QMessageBox::Ok );
    } else {
        QVector< QVector< int > > solved_numbers = solver.getSolution();

        // enter solution in missing boxes
        for(int i=0; i<NUM_ENTRIES; i++) {
            for(int j=0; j<NUM_ENTRIES; j++) {
                QLineEdit *cell = boxes[i][j];

                if(cell && cell->text().isEmpty()) {
                    QString const solved_value = QString::number( solved_numbers[i][j] ); // FIXME: can this fail?
                    cell->setText( solved_value );

                    //make cell green
                    QPalette cell_pallete = cell->palette();
                    cell_pallete.setColor( QPalette::Base, QColor(0,255,0) );
                    cell->setPalette( cell_pallete );
                }
            }
        }
    }
}

} // namespace sudoku
