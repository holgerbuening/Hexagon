#include "unittype.h"


QMap<UnitType::Type, QPixmap> UnitType::pixmaps;
QMap<UnitType::Type, int> UnitType::ranges;
QMap<UnitType::Type, int> UnitType::territory;
QMap<UnitType::Type, int> UnitType::offense;
QMap<UnitType::Type, int> UnitType::defense;
QMap<UnitType::Type, int> UnitType::attackRange;

void UnitType::loadUnits()
{
    //infantry
    pixmaps[infantry] = QPixmap(":/Images/infantry.png");
    ranges[infantry]=3;
    territory[infantry]=0;
    offense[infantry]=100;
    defense[infantry]=100;
    attackRange[infantry]=1;

    //military base
    pixmaps[militarybase] = QPixmap(":/Images/militarybase.png");
    ranges[militarybase]=0;
    territory[militarybase]=0;
    offense[militarybase]=0;
    defense[militarybase]=10;
    attackRange[militarybase]=0;

    //machine gun unit
    pixmaps[machineGun] = QPixmap(":/Images/MachineGunUnit.png");
    ranges[machineGun]=2;
    territory[machineGun]=0;
    offense[machineGun]=150;
    defense[machineGun]=100;
    attackRange[machineGun]=1;

}

QString UnitType::getName(UnitType::Type type)
{
    switch (type)
    {
        case infantry: return "Infantry";
        case militarybase: return "Headquarter";
        case machineGun: return "Machine gun";
        default: return "Unknown";
    }
}
