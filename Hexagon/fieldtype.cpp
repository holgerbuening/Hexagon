#include "fieldtype.h"
#include <QPixmap>

// Definition der statischen Variablen
QMap<FieldType::Type, QPixmap> FieldType::pixmaps;
QMap<FieldType::Type, int> FieldType::territory;
QMap<FieldType::Type, int> FieldType::movementCost;

void FieldType::loadPixmaps() {
    pixmaps[Woods] = QPixmap(":/hexfields/Images/Forest_big.png");
    territory[Woods]=0;
    movementCost[Woods]=2;
    pixmaps[Ocean] = QPixmap(":/hexfields/Images/sea_big.png");
    territory[Ocean]=1;
    movementCost[Ocean]=1;
    pixmaps[Mountain] = QPixmap(":/hexfields/Images/mountain_big.png");
    territory[Mountain]=0;
    movementCost[Mountain]=3;
    pixmaps[Farmland] = QPixmap(":/hexfields/Images/farmland_big.png");
    territory[Farmland]=0;
    movementCost[Farmland]=1;
    // Füge hier weitere Pixmaps hinzu
}
QString FieldType::fieldTypeToString(FieldType::Type type)
{
    switch (type)
    {
        case Woods: return "Woods";
        case Ocean: return "Ocean";
        case Mountain: return "Mountain";
        case Farmland: return "Farmland";
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
