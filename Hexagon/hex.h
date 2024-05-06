#ifndef HEX_H
#define HEX_H

#include <QPixmap>
#include "fieldtype.h"

class Hex {
public:
    int getRow();
    int getCol();
    FieldType::Type getFieldType();
    const QString getFieldTypeText();
    void setFieldType(FieldType::Type fieldType);
    int getMovementCost();
    const QPixmap& getPixmap();
    Hex(int col=0, int row=0, FieldType::Type fieldType=FieldType::Woods, int movementCost=1);
private:
    int col, row;
    FieldType::Type fieldType;
    int movementCost;




};

#endif // HEX_H
