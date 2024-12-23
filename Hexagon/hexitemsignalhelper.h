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


#ifndef HEXITEMSIGNALHELPER_H
#define HEXITEMSIGNALHELPER_H

#include <QObject>

class HexItem;  // Forward-declaration

class HexItemSignalHelper : public QObject {
    Q_OBJECT
public:
    HexItemSignalHelper(QObject *parent = nullptr);
signals:
    void itemSelected(HexItem *item);
};

#endif // HEXITEMSIGNALHELPER_H
