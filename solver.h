#ifndef SOLVER_H
#define SOLVER_H

#include "common.h"

#include <QVector>
#include <QSet>

class Decision;
template <class T> class QList;

class Solver
{
public:
    Solver(int const fixed_numbers[ENTRIES][ENTRIES]);
    ~Solver();

    bool solve();
    QVector< QVector< int > > getSolution();

private:
    bool recursiveSolve(int depth);

    void setup();
    bool checkValid(bool &addedDecision, QList<Decision> &decisions);
    bool checkFilled();

    void reverseDecisions( QList<Decision> const &decisions );

private:
    QVector< QVector< int > > fixed_numbers;
    QVector< QVector< int > > used_numbers;
    QVector< QSet<int> > blocks;

    QVector< QVector< QSet<int> > > numbers_to_try;
};

#endif // SOLVER_H
