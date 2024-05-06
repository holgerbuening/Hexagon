// hex.cpp
#include "hex.h"


Hex::Hex(int col_v, int row_v, FieldType::Type fieldType_v, int movementCost_V)
{
    col=col_v;
    row=row_v;
    fieldType=fieldType_v;
    movementCost=movementCost_V;
}
 //   : col(col), row(row), fieldType(fieldType), movementCost(movementCost) {}
int Hex::getRow()
{
    return row;
}
int Hex::getCol()
{
    return col;
}
FieldType::Type Hex::getFieldType()
{
    return fieldType;
}
int Hex::getMovementCost()
{
    return movementCost;
}
const QPixmap& Hex::getPixmap()
{
    return FieldType::getPixmap(fieldType);
}
void Hex::setFieldType(FieldType::Type fieldType_v)
{
    fieldType=fieldType_v;
}
const QString Hex::getFieldTypeText()
{
    return FieldType::fieldTypeToString(fieldType);
}
