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


#include "hexitem.h"
#include "hexitemsignalhelper.h"
#include <QPixmap>


HexItem::HexItem (const QPixmap& pixmap, QGraphicsItem* parent, int row_v, int col_v)
    : QGraphicsPixmapItem(pixmap, parent)
{
    row=row_v;
    col=col_v;
    signalHelper = new HexItemSignalHelper(nullptr);
    overlayPixmap= QPixmap(":/hexfields/Images/grid_big_selected.png");
    overlayItem=nullptr;
}

HexItem::~HexItem()
{
    if (signalHelper)
    {
        delete signalHelper;
    }
}


void HexItem::mousePressEvent(QGraphicsSceneMouseEvent* event)
{
    if (event->button() == Qt::LeftButton)
        {
                overlayPixmap = overlayPixmap.scaled(this->boundingRect().size().toSize());
                overlayItem = new QGraphicsPixmapItem(overlayPixmap, this);
                emit signalHelper->itemSelected(this);
        }
}
void HexItem::sendItemSelected()
{
    emit signalHelper->itemSelected(this);
}
int HexItem::getrow()
{
    return row;
}
int HexItem::getcol()
{
    return col;
}

void HexItem::deleteOverlayItem()
{
    if (overlayItem)
    {
        delete overlayItem;
        overlayItem=nullptr;
    }
}
