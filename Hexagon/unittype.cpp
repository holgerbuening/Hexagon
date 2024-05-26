#include "unittype.h"


QMap<UnitType::Type, QPixmap> UnitType::pixmaps;
QMap<UnitType::Type, int> UnitType::ranges;
QMap<UnitType::Type, int> UnitType::territory;
QMap<UnitType::Type, int> UnitType::offense;
QMap<UnitType::Type, int> UnitType::defense;
QMap<UnitType::Type, int> UnitType::attackRange;

void UnitType::loadUnits() {
    pixmaps[infantry] = QPixmap(":/Images/infantry.png");
    ranges[infantry]=2;
    territory[infantry]=0;
    offense[infantry]=100;
    defense[infantry]=100;
    attackRange[infantry]=1;


}

QString UnitType::getName(UnitType::Type type)
{
    switch (type)
    {
        case infantry: return "Infantry";
        default: return "Unknown";
    }
}
