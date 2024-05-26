#ifndef UNIT_H
#define UNIT_H

#include "unittype.h"

class Unit
{
public:
    Unit (UnitType::Type,int row, int col, QString country);


    UnitType::Type getType() const { return unitType; }
    const QString getUnitTypeText();
    int getRow() { return row; }
    int getCol() { return col; }
    int getTerritory() {return territory;}
    int getCurrentState() { return currentState; }
    int getRemainingMovementPoints() { return remainingMovementPoints; }
    QString getCountry() {return country;}

    void setRow(int newRow) { row = newRow; }
    void setCol(int newCol) { col = newCol; }
    void setCurrentState(int newState) { currentState = newState; }
    void setRemainingMovementPoints(int points) { remainingMovementPoints = points; }
    void setCountry(QString country_v) {country=country_v;}

    void moveTo(int newRow, int newCol, int distance);

private:
    UnitType::Type unitType;
    QString country;
    int row;
    int col;
    int currentState;
    int remainingMovementPoints;
    int territory;
};

#endif // UNIT_H
