// hex.cpp
#include "hex.h"


Hex::Hex(int col_v, int row_v, FieldType::Type fieldType_v)
{
    col=col_v;
    row=row_v;
    fieldType=fieldType_v;
    movementCost=FieldType::getMovementCost(fieldType);
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
    return movementCost;
}
void Hex::setMovementCost()
{
    movementCost=FieldType::getMovementCost(fieldType);
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
