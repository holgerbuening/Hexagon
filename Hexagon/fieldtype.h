#ifndef FIELDTYPE_H
#define FIELDTYPE_H

#include <QPixmap>
#include <QString>
#include <QMap>

class FieldType {
public:
    enum Type {
        Woods,
        Ocean,
        Mountain,
        Farmland
        // weitere Feldtypen
    };
    static QString fieldTypeToString(FieldType::Type type);
    static void loadPixmaps();

    static const QPixmap& getPixmap(Type type) {
        return pixmaps[type];
    }
    static int getTerritory(Type type);
    static int getMovementCost(Type type);
    static int getDefense(Type type);

private:
    static QMap<Type, QPixmap> pixmaps;
    static QMap<Type, int> territory; //0=land, 1=water, 99=all
    static QMap<Type, int> movementCost;
    static QMap<Type, int> defense;

};

#endif // FIELDTYPE_H
