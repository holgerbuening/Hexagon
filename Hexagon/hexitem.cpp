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
