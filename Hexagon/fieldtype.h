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

private:
    static QMap<Type, QPixmap> pixmaps;

public:
    static void loadPixmaps();

    static const QPixmap& getPixmap(Type type) {
        return pixmaps[type];
    }
};

#endif // FIELDTYPE_H
