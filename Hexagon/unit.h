#ifndef UNIT_H
#define UNIT_H

#include "unittype.h"
#include "hex.h"

enum AIState
{
    ATTACK,
    DEFEND,
    CAPTURE,
    RETREAT
};


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
    bool getActed() {return acted;}
    QString getCountry() {return country;}
    AIState getAiState() {return aiState;}

    void setRow(int newRow) { row = newRow; }
    void setCol(int newCol) { col = newCol; }
    void setCurrentState(int newState) { currentState = newState; }
    void increaseExperience();
    void setExperience(int newExperience);
    void setOffense(int newOffense){offense=newOffense;}
    void setDefense(int newDefense){defense=newDefense;}
    void setAttackrange(int newAttackRange){attackRange=newAttackRange;}
    void setRemainingMovementPoints(int points) { remainingMovementPoints = points; }
    void setCountry(QString newCountry) {country=newCountry;}
    void setActed();
    void deleteActed();
    void setAiState(AIState newState) {aiState=newState;}
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
    bool acted; //true=no further movements or attacks
    AIState aiState;
};

#endif // UNIT_H
