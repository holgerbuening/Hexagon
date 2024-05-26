#include "unit.h"
#include "unittype.h"

Unit::Unit (UnitType::Type unitType_v, int row_v, int col_v, QString country_v)
{
    unitType=unitType_v;
    row=row_v;
    col=col_v;
    currentState=100;
    country=country_v;
    remainingMovementPoints=UnitType::getRange(unitType);
    territory=UnitType::getTerritory(unitType);
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
