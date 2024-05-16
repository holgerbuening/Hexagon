#ifndef UNIT_H
#define UNIT_H

#include "unittype.h"

class Unit
{
public:
    Unit(const UnitType &type, int row, int col)
        : type(type), row(row), col(col), currentState(100), remainingMovementPoints(type.getMobility()) {}



    const UnitType& getType() const { return type; }
    int getRow() const { return row; }
    int getCol() const { return col; }
    int getCurrentState() const { return currentState; }
    int getRemainingMovementPoints() const { return remainingMovementPoints; }

    void setRow(int newRow) { row = newRow; }
    void setCol(int newCol) { col = newCol; }
    void setCurrentState(int newState) { currentState = newState; }
    void setRemainingMovementPoints(int points) { remainingMovementPoints = points; }

    void moveTo(int newRow, int newCol);

private:
    const UnitType &type;
    int row;
    int col;
    int currentState;
    int remainingMovementPoints;
};

#endif // UNIT_H
