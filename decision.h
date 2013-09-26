#ifndef DECISION_H
#define DECISION_H

namespace sudoku {

// Represents a reversable decision for the backracking algorithm.
class Decision
{
public:
    explicit Decision(int _x, int _y, int _value)
    	: x(_x)
    	, y(_y)
    	, value(_value)
	{}

    int getX() const { return x; }
    int getY() const { return y; }
    int getValue() const { return value; }

private:
    int x;
    int y;
    int value;
};

} // namespace sudoku

#endif // DECISION_H
