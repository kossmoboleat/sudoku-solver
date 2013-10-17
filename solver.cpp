#include "solver.h"

#include <stdlib.h>
#include <iostream>

#include "decision.h"
#include "logger.h"

namespace sudoku {

Solver::Solver( QVector< QVector<int> > const in_fixed_numbers )
{
    for(int i=0; i<NUM_ENTRIES; i++) {
        QVector<int> row;
        for(int j=0; j<NUM_ENTRIES; j++) {
            row.push_back( in_fixed_numbers[i][j] );
        }
        fixed_numbers.push_back(row);
    }
}

Solver::~Solver()
{
}

void Solver::setup()
{
    // setup a set with all numbers
    QSet< int > all_numbers;
    for( int i=1; i<=NUM_ENTRIES; i++ ) {
        all_numbers.insert( i );
    }

    // initialize blocks to have all numbers
    for( int i=0; i<NUM_ENTRIES; i++ ) {
        blocks.push_back( all_numbers );
    }

    // convert integer array to two-dimensional QVector
    for( int i=0; i<NUM_ENTRIES; i++ ) {

        QVector< int > row;
        QVector< QSet< int > > numbers_to_try_row;
        for( int j=0; j<NUM_ENTRIES; j++ ) {
            int const fixed_number = fixed_numbers[i][j];

            row.push_back(fixed_number);

            QSet<int> emptySet;
            numbers_to_try_row.push_back( emptySet );

            int const block_number = SIDE*(i/SIDE) + (j/SIDE);
            blocks[block_number].remove( fixed_number );
        }

        used_numbers.push_back( row );
        numbers_to_try.push_back( numbers_to_try_row );
    }
}

bool Solver::solve()
{
    setup();

    // check if starting numbers are valid
    QList<Decision> decisions;
    bool addedDecisions=false;

    if( !isValid(addedDecisions, decisions) ) {
        std::cerr << "Starting numbers are invalid!" << std::endl;

        reverseDecisions( decisions );

        return false;
    }

    return recursiveSolve(0);
}

bool Solver::recursiveSolve( int depth )
{
    if( isFilled() ) {
        return true;
    }

    Logger::getInstance().log( "***** recursing to depth " + QString::number( depth ) + " *****" );

    QList<Decision> decisions;

    bool addedDerivations;

    do {
        addedDerivations=false;

        if( !isValid(addedDerivations, decisions) ) {
            Logger::getInstance().log( "Not a valid solution." );
            reverseDecisions( decisions );
            return false;
        }

    } while( addedDerivations );

    if( isFilled() ) {
        return true;
    }

    int row=0;
    int column=0;

    bool found_empty_cell=false;

    for(int i=0; i<NUM_ENTRIES && !found_empty_cell; i++ ) {
        for(int j=0;j<NUM_ENTRIES; j++ ) {
            if( used_numbers[i][j]==0 ) {
                row = i;
                column = j;

                found_empty_cell = true;

                break;
            }
        }
    }

    if( !found_empty_cell ) {
        Logger::getInstance().log( "Did not find empty cell." );

        reverseDecisions( decisions );

        return false;
    } else {
        QSet<int> numbers_to_try_cell( numbers_to_try[row][column] );
        for( QSet<int>::const_iterator const_it=numbers_to_try_cell.begin(); const_it!=numbers_to_try_cell.end(); const_it++) {

            Decision candidate(row,column,*const_it);
            decisions.push_back(candidate);

            used_numbers[row][column]=*const_it;

            Logger::getInstance().log( "Trying " + QString::number( *const_it ) + " at " + QString::number( row ) + "," + QString::number( column ) );

            if( recursiveSolve(depth+1) ) {
                return true;
            } else {
                Logger::getInstance().log( "***** unsuccessful: back at depth " + QString::number( depth ) + " *****" );

                // removing try
                used_numbers[row][column]=0;
                decisions.pop_back();
            }
        }
    }

    // backtracking to higher recursion level
    reverseDecisions( decisions );
    return false;
}

void Solver::reverseDecisions( QList<Decision> const & decisions )
{
    for(QList<Decision>::const_iterator const_it=decisions.begin(); const_it!=decisions.end(); const_it++ ) {
        used_numbers[const_it->getX()][const_it->getY()] = 0;
    }
}

bool Solver::isFilled() const
{
    for(int i=0; i<NUM_ENTRIES; i++) {
        for(int j=0; j<NUM_ENTRIES; j++) {
            if( used_numbers[i][j]==0 ) {
                return false;
            }
        }
    }
    return true;
}

bool Solver::isValid( bool & addedDecision, QList<Decision> & decisions )
{
    QSet< int > all_numbers;
    for( int i=1; i<=NUM_ENTRIES; i++ ) {
        all_numbers.insert(i);
    }

    // check rows
    QVector< QSet< int > > checked_rows;
    for( int i=0; i<NUM_ENTRIES; i++ ) {

        // for every row check that a number does not occur twice
        QSet< int > numbers_to_check( all_numbers );
        for( int j=0; j<NUM_ENTRIES; j++) {
            if( used_numbers[i][j]!=0 && !numbers_to_check.remove( used_numbers[i][j] ) ) { // number already seen in the row
                Logger::getInstance().log( "Checking Rows: Cell " + QString::number( i ) + "," + QString::number( j ) + " value: " + QString::number( used_numbers[i][j] ) + " is not valid!");

                return false;
            }
        }
        checked_rows.push_back( numbers_to_check );
    }

    // check columns
    QVector< QSet< int > > checked_columns;
    for( int j=0; j<NUM_ENTRIES; j++ ) {

        // for every column check that a number does not occur twice
        QSet< int > numbers_to_check( all_numbers );
        for( int i=0; i<NUM_ENTRIES; i++) {

            if( used_numbers[i][j]!=0 && !numbers_to_check.remove( used_numbers[i][j] ) ) { // number already seen in this column
                Logger::getInstance().log( "Checking Columns: Cell " + QString::number( i ) + "," + QString::number( j ) + " value: " + QString::number( used_numbers[i][j] ) + " is not valid!" );

                return false;
            }
        }
        checked_columns.push_back( numbers_to_check );
    }

    // check blocks
    QVector< QSet<int> > checked_blocks;
    for( int i=0; i<NUM_ENTRIES; i++ ) {
        checked_blocks.push_back( all_numbers );
    }

    for( int i=0; i<NUM_ENTRIES; i++ ) {
        for( int j=0; j<NUM_ENTRIES; j++ ) {
            if( used_numbers[i][j]!=0 ) {
                int const block_number = SIDE*(i/SIDE) + (j/SIDE);
                if( !checked_blocks[block_number].remove( used_numbers[i][j] ) ) { // number already seen in this block

                    Logger::getInstance().log( "Checking Blocks: Cell " + QString::number( i ) + "," + QString::number( j ) + " value: " + QString::number( used_numbers[i][j] ) + " is not valid!" );
                    return false;
                }
            }
        }
    }

    for( int i=0; i<NUM_ENTRIES; i++ ) {
        for( int j=0; j<NUM_ENTRIES; j++ ) {
            if( used_numbers[i][j]==0 ){
                int const block_number = SIDE*(i/SIDE) + (j/SIDE);

                numbers_to_try[i][j] = checked_rows.at(i);
                numbers_to_try[i][j] = numbers_to_try[i][j].intersect( checked_columns[j]);
                numbers_to_try[i][j] = numbers_to_try[i][j].intersect( checked_blocks[block_number] );

                if( numbers_to_try[i][j].size()==0 ) {
                    Logger::getInstance().log( "Cell " + QString::number( i ) + "," + QString::number( j ) + " has no solutions left!");

                    return false;
                } else if( numbers_to_try[i][j].size()==1 ) {
                    addedDecision = true;

                    int const solved_value = *numbers_to_try[i][j].begin();
                    Logger::getInstance().log( "Found value " + QString::number( solved_value ) + " at cell " + QString::number( i ) + "," + QString::number( j ) );

                    used_numbers[i][j] = solved_value;
                    Decision decision(i,j,solved_value);
                    decisions.push_back( decision );
                }
            }
        }
    }

    return true;
}

QVector< QVector< int > > Solver::getSolution()
{
    return used_numbers;
}

} // namespace sudoku
