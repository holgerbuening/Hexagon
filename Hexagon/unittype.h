#ifndef UNITTYPE_H
#define UNITTYPE_H

#include <QString>
#include <QPixmap>

class UnitType
{
public:
    UnitType(const QString &name, int offense, int defense, int mobility, int range, const QPixmap &pixmap)
        : name(name), offense(offense), defense(defense), mobility(mobility), range(range), pixmap(pixmap) {}

    QString getName() const { return name; }
    int getOffense() const { return offense; }
    int getDefense() const { return defense; }
    int getMobility() const { return mobility; }
    int getRange() const { return range; }
    QPixmap getPixmap() const { return pixmap; }

private:
    QString name;
    int offense;
    int defense;
    int mobility;
    int range;
    QPixmap pixmap;
};


#endif // UNITTYPE_H
