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



private:
    QPixmap overlayPixmap;
    HexItemSignalHelper *signalHelper; // Besitzt die Signal-Funktionalität
    int row;
    int col;

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent* event) override;
};

#endif // HEXITEM_H
