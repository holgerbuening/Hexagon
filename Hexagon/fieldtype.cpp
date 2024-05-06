#include "fieldtype.h"
#include <QPixmap>

// Definition der statischen Variablen
QMap<FieldType::Type, QPixmap> FieldType::pixmaps;

void FieldType::loadPixmaps() {
    pixmaps[Woods] = QPixmap(":/hexfields/Images/Forest_big.png");
    pixmaps[Ocean] = QPixmap(":/hexfields/Images/sea_big.png");
    pixmaps[Mountain] = QPixmap(":/hexfields/Images/mountain_big.png");
    pixmaps[Farmland] = QPixmap(":/hexfields/Images/farmland_big.png");
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
