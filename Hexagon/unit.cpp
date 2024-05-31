#include "unit.h"
#include "unittype.h"

Unit::Unit (UnitType::Type unitType_v, int row_v, int col_v, QString country_v)
{
    unitType=unitType_v;
    row=row_v;
    col=col_v;
    currentState=100;
    experience=0;
    country=country_v;
    acted=false;
    remainingMovementPoints=UnitType::getRange(unitType);
    territory=UnitType::getTerritory(unitType);
    offense=UnitType::getOffense(unitType);
    defense=UnitType::getDefense(unitType);
    attackRange=UnitType::getAttackRange(unitType);
}

void Unit::moveTo(int newRow, int newCol, int distance)
{
    setRow(newRow);
    setCol(newCol);
    setRemainingMovementPoints (getRemainingMovementPoints()-distance);
}

const QString Unit::getUnitTypeText()
{
    return UnitType::getName(unitType);
}

void Unit::setExperience(int newExperience)
{
    experience=newExperience;
    if (experience>10) {experience=10;}
}

void Unit::increaseExperience()
{
    experience++;
    if (experience >10) {experience=10;}
}

void Unit::setActed()
{
    acted=true;
}

void Unit::deleteActed()
{
    acted=false;
}
