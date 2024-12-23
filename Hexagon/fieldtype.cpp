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


#include "fieldtype.h"
#include <QPixmap>

// Definition of static members
QMap<FieldType::Type, QPixmap> FieldType::pixmaps;
QMap<FieldType::Type, int> FieldType::territory;
QMap<FieldType::Type, int> FieldType::movementCost;
QMap<FieldType::Type, int> FieldType::defense;

void FieldType::loadPixmaps() {
    pixmaps[Woods] = QPixmap(":/hexfields/Images/Forest_big.png");
    territory[Woods]=0;
    movementCost[Woods]=2;
    defense[Woods]=35;
    pixmaps[Ocean] = QPixmap(":/hexfields/Images/sea_big.png");
    territory[Ocean]=1;
    movementCost[Ocean]=1;
    defense[Ocean]=0;
    pixmaps[Mountain] = QPixmap(":/hexfields/Images/mountain_big.png");
    territory[Mountain]=0;
    movementCost[Mountain]=3;
    defense[Mountain]=50;
    pixmaps[Farmland] = QPixmap(":/hexfields/Images/farmland_big.png");
    territory[Farmland]=0;
    movementCost[Farmland]=1;
    defense[Farmland]=15;
    pixmaps[Hills] = QPixmap(":/hexfields/Images/hills_big.png");
    territory[Hills]=0;
    movementCost[Hills]=2;
    defense[Hills]=35;
    // add further field types
}
QString FieldType::fieldTypeToString(FieldType::Type type)
{
    switch (type)
    {
        case Woods: return "Woods";
        case Ocean: return "Ocean";
        case Mountain: return "Mountain";
        case Farmland: return "Farmland";
        case Hills: return "Hills";
        default: return "Unknown";
    }
}

int FieldType::getTerritory(Type type)
{
return territory[type];
}

int FieldType::getMovementCost(Type type)
{
    return movementCost[type];
}

int FieldType::getDefense(Type type)
{
    return defense[type];
}
