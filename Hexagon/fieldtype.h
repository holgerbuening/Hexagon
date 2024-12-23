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
        Farmland,
        Hills,
        Count //number of field types
        // add further field types
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
