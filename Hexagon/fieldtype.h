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

private:
    static QMap<Type, QPixmap> pixmaps;
    static QMap<Type, int> territory; //0=land, 1=water

};

#endif // FIELDTYPE_H
