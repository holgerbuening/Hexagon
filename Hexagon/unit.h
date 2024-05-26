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
    int getExperience(){return experience;}
    int getOffense(){return offense;}
    int getDefense(){return defense;}
    int getAttackRange(){return attackRange;}
    int getRemainingMovementPoints() { return remainingMovementPoints; }
    QString getCountry() {return country;}

    void setRow(int newRow) { row = newRow; }
    void setCol(int newCol) { col = newCol; }
    void setCurrentState(int newState) { currentState = newState; }
    void setExperience(int newExperience);
    void setOffense(int newOffense){offense=newOffense;}
    void setDefense(int newDefense){defense=newDefense;}
    void setAttackrange(int newAttackRange){attackRange=newAttackRange;}
    void setRemainingMovementPoints(int points) { remainingMovementPoints = points; }
    void setCountry(QString newCountry) {country=newCountry;}

    void moveTo(int newRow, int newCol, int distance);

private:
    UnitType::Type unitType;
    QString country;
    int row;
    int col;
    int currentState;
    int experience;
    int offense;
    int defense;
    int attackRange;
    int remainingMovementPoints;
    int territory;
};

#endif // UNIT_H
