#include "unittype.h"


QMap<UnitType::Type, QPixmap> UnitType::pixmaps;
QMap<UnitType::Type, int> UnitType::ranges;

void UnitType::loadUnits() {
    pixmaps[infantry] = QPixmap(":/Images/infantry.png");
    ranges[infantry]=2;
    // Füge hier weitere Pixmaps hinzu

}

QString UnitType::getName(UnitType::Type type)
{
    switch (type)
    {
        case infantry: return "Infantry";
        default: return "Unknown";
    }
}
