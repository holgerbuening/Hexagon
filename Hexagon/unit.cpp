#include "unit.h"
#include "unittype.h"

Unit::Unit (UnitType::Type unitType_v, int row_v, int col_v)
{
    unitType=unitType_v;
    row=row_v;
    col=col_v;
    currentState=100;
    remainingMovementPoints=UnitType::getRange(unitType);
}

void Unit::moveTo(int newRow, int newCol) {
    setRow(newRow);
    setCol(newCol);
    setRemainingMovementPoints (0); // Assuming a single move consumes all movement points
}

const QString Unit::getUnitTypeText()
{
    return UnitType::getName(unitType);
}
