#include "unit.h"

void Unit::moveTo(int newRow, int newCol) {
    setRow(newRow);
    setCol(newCol);
    setRemainingMovementPoints (0); // Assuming a single move consumes all movement points
}
