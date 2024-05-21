#ifndef UNITTYPE_H
#define UNITTYPE_H

#include <QString>
#include <QPixmap>
#include <QMap>

class UnitType
{
public:
    enum Type {
        infantry
        // weitere Truppentypen
    };
    static void loadUnits();

    static QString getName(UnitType::Type type); /*
    int getOffense() const { return offense; }
    int getDefense() const { return defense; }
    int getMobility() const { return mobility; }*/
    static int getRange(UnitType::Type type) { return ranges[type]; }
    static const QPixmap& getPixmap(Type type) {return pixmaps[type];}
private:
    static QMap<Type, QPixmap> pixmaps;
    static QMap<Type, int> ranges;
    QString name;
    //int offense;
    //int defense;
    //int mobility;
    //const QPixmap pixmap;
};


#endif // UNITTYPE_H
