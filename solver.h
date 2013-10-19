#pragma once

#include "common.h"

#include <QtCore/QVector>
#include <QtCore/QSet>
#include <QtCore/QList>

namespace sudoku {

class Decision;

// The actual workflow class for solving a sudoku.
class Solver
{
public:
    explicit Solver( QVector< QVector<int> > const fixed_numbers );
    ~Solver();

    bool solve();
    QVector< QVector< int > > getSolution();

private:
    bool recursiveSolve( int depth );

    void setup();

    bool isValid( bool & addedDecision, QList<Decision> & decisions );
    bool isValidRow( QVector< QVector<int> > const &rFilledGrid, int const row, QSet< int > &rUnusedNumbers ) const;
    bool isValidColumn( QVector< QVector<int> > const &rFilledGrid, int const col, QSet< int > &rUnusedNumbers ) const;
    bool isValidBlock( QVector< QVector<int> > const &rFilledGrid, int const block, QSet< int > &rUnusedNumbers ) const;

    bool isFilled() const;

    void reverseDecisions( QList<Decision> const & decisions );

private:
    QVector< QVector< int > > fixed_numbers;
    QVector< QVector< int > > filled_grid;
    QVector< QSet<int>    >   blocks;

    QVector< QVector< QSet<int> > > numbers_to_try;

};

} // namespace sudoku

