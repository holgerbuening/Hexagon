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


#ifndef HEXITEM_H
#define HEXITEM_H

#include <QGraphicsSceneMouseEvent>
#include <QGraphicsPixmapItem>
#include "hexitemsignalhelper.h"


class HexItem : public QGraphicsPixmapItem
{

public:
    HexItem(const QPixmap& pixmap, QGraphicsItem* parent = nullptr, int row=0, int col=0);
    ~HexItem();
    QGraphicsPixmapItem* overlayItem;
    // Methode zum Senden von Signalen
    void sendItemSelected();
    HexItemSignalHelper* getSignalHelper() const { return signalHelper; }
    int getrow();
    int getcol();
    void deleteOverlayItem();



private:
    QPixmap overlayPixmap;
    HexItemSignalHelper *signalHelper; // Besitzt die Signal-Funktionalität
    int row;
    int col;

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent* event) override;
};

#endif // HEXITEM_H
