/*
 * This file is part of Hexagon
 *
 * Hexagon is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Hexagon is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Hexagon. If not, see <https://www.gnu.org/licenses/>.
 */


#ifndef UNITTYPE_H
#define UNITTYPE_H

#include <QString>
#include <QPixmap>
#include <QMap>

class UnitType
{
public:
    enum Type {
        infantry,
        militarybase,
        machineGun,
        medic
                // add further Unit types
    };
    static void loadUnits();

    static QString getName(UnitType::Type type);
    static int getRange(UnitType::Type type) { return ranges[type]; }
    static int getTerritory(UnitType::Type type) { return territory[type]; }
    static int getOffense(UnitType::Type type) {return offense[type];}
    static int getDefense(Type type) {return defense[type];}
    static int getAttackRange(Type type) {return attackRange[type];}
    static int getPrice(Type type) {return price[type];}
    static const QPixmap& getPixmap(Type type) {return pixmaps[type];}


private:
    static QMap<Type, QPixmap> pixmaps;
    static QMap<Type, int> ranges;
    static QMap<Type, int> territory;//0=land, 1=water, 2=air
    static QMap<Type, int> offense;
    static QMap<Type, int> defense;
    static QMap<Type, int> attackRange;
    static QMap<Type, int> price;
    QString name;

};


#endif // UNITTYPE_H
