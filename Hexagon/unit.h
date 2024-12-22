/*
 * This file is part of Hexagon
 *
 * Hexagon is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Hexagon is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Hexagon. If not, see <https://www.gnu.org/licenses/>.
 */


#ifndef UNIT_H
#define UNIT_H

#include "unittype.h"
#include "hex.h"
#include <QDataStream>

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
    Unit (UnitType::Type type= UnitType::infantry,int row=0, int col=0, QString country="");


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

    friend QDataStream& operator<<(QDataStream& out, const Unit& unit);
    friend QDataStream& operator>>(QDataStream& in, Unit& unit);

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

QDataStream& operator<<(QDataStream& out, const Unit& unit);
QDataStream& operator>>(QDataStream& in, Unit& unit);

#endif // UNIT_H
