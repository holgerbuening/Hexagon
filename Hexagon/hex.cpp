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


// hex.cpp
#include "hex.h"


Hex::Hex(int col_v, int row_v, FieldType::Type fieldType_v)
{
    col=col_v;
    row=row_v;
    fieldType=fieldType_v;
    movementCost=FieldType::getMovementCost(fieldType);
    defense=FieldType::getDefense(fieldType);
}
 //   : col(col), row(row), fieldType(fieldType), movementCost(movementCost) {}

int Hex::getRow() const
{
    return row;
}



int Hex::getCol() const
{
    return col;
}

FieldType::Type Hex::getFieldType() const
{
    return fieldType;
}

int Hex::getMovementCost() const
{
    if (hasRoad) 
    {
        return movementCost / 2;  // Example: reduce cost by half if there's a road
    }
    return movementCost;
}

void Hex::setMovementCost()
{
    movementCost=FieldType::getMovementCost(fieldType);
}

void Hex::setMovementCost(int newMovementCost)
{
    movementCost=newMovementCost;
}

int Hex::getDefense() const
{
    return defense;
}

void Hex::setDefense(int newDefense)
{
    defense=newDefense;
}


const QPixmap& Hex::getPixmap()
{
    return FieldType::getPixmap(fieldType);
}

void Hex::setFieldType(FieldType::Type fieldType_v)
{
    fieldType=fieldType_v;
}

const QString Hex::getFieldTypeText() const
{
    return FieldType::fieldTypeToString(fieldType);
}

int Hex::getTerritory()
{
    return FieldType::getTerritory(fieldType);
}

bool Hex::getHasRoad() const
{
    return hasRoad;
}

void Hex::setHasRoad(bool val)
{
    hasRoad = val;
}

QDataStream& operator<<(QDataStream& out, const Hex& hex) {
    out << hex.row << hex.col << static_cast<int>(hex.fieldType) << hex.movementCost << hex.defense;
    return out;
}

QDataStream& operator>>(QDataStream& in, Hex& hex) {
    int fieldType;
    in >> hex.row >> hex.col >> fieldType >> hex.movementCost >> hex.defense;
    hex.fieldType = static_cast<FieldType::Type>(fieldType);
    return in;
}
