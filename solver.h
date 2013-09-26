#ifndef SOLVER_H
#define SOLVER_H

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
    bool isFilled() const;

    void reverseDecisions( QList<Decision> const & decisions );

private:
    QVector< QVector< int > > fixed_numbers;
    QVector< QVector< int > > used_numbers;
    QVector< QSet<int>    >   blocks;

    QVector< QVector< QSet<int> > > numbers_to_try;
};

} // namespace sudoku

#endif // SOLVER_H
