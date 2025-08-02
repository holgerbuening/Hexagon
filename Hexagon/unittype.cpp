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


#include "unittype.h"


QMap<UnitType::Type, QPixmap> UnitType::pixmaps;
QMap<UnitType::Type, int> UnitType::ranges;
QMap<UnitType::Type, int> UnitType::territory;
QMap<UnitType::Type, int> UnitType::offense;
QMap<UnitType::Type, int> UnitType::defense;
QMap<UnitType::Type, int> UnitType::attackRange;
QMap<UnitType::Type, int> UnitType::price;

void UnitType::loadUnits()
{
    //infantry
    pixmaps[infantry] = QPixmap(":/Images/infantry.png");
    ranges[infantry]=3;
    territory[infantry]=0;
    offense[infantry]=100;
    defense[infantry]=100;
    attackRange[infantry]=1;
    price[infantry]=150;

    //military base
    pixmaps[militarybase] = QPixmap(":/Images/militarybase.png");
    ranges[militarybase]=0;
    territory[militarybase]=0;
    offense[militarybase]=0;
    defense[militarybase]=10;
    attackRange[militarybase]=0;
    price[militarybase]=0;

    //machine gun unit
    pixmaps[machineGun] = QPixmap(":/Images/MachineGunUnit.png");
    ranges[machineGun]=2;
    territory[machineGun]=0;
    offense[machineGun]=150;
    defense[machineGun]=100;
    attackRange[machineGun]=1;
    price[machineGun]=250;

    //medic
    pixmaps[medic] = QPixmap(":/Images/medic.png");
    ranges[medic]=3;
    territory[medic]=0;
    offense[medic]=0;
    defense[medic]=10;
    attackRange[medic]=1;
    price[medic]=100;

    //field artillery
    pixmaps[fieldArtillery] = QPixmap(":/Images/fieldartillery.png");
    ranges[fieldArtillery]=2;
    territory[fieldArtillery]=0;
    offense[fieldArtillery]=200;
    defense[fieldArtillery]=50;
    attackRange[fieldArtillery]=3;
    price[fieldArtillery]=300;

    //cavalry
    pixmaps[cavalry] = QPixmap(":/Images/cavalry.png");
    ranges[cavalry]=5;
    territory[cavalry]=0;
    offense[cavalry]=120;
    defense[cavalry]=50;
    attackRange[cavalry]=1;
    price[cavalry]=200;

    //tank
    pixmaps[tank] = QPixmap(":/Images/tank.png");
    ranges[tank]=2;
    territory[tank]=0;
    offense[tank]=200;
    defense[tank]=150;
    attackRange[tank]=2;
    price[tank]=400;

    //engineer
    pixmaps[engineer] = QPixmap(":/Images/engineer.png");
    ranges[engineer]=3;
    territory[engineer]=0;
    offense[engineer]=0;
    defense[engineer]=10;
    attackRange[engineer]=1;
    price[engineer]=180;

}

QString UnitType::getName(UnitType::Type type)
{
    switch (type)
    {
        case infantry: return "Infantry";
        case militarybase: return "Headquarter";
        case machineGun: return "Machine gun";
        case medic: return "Medic";
        case fieldArtillery: return "Field artillery";
        case cavalry: return "Cavalry";
        case tank: return "Tank";
        default: return "Unknown";
    }
}
