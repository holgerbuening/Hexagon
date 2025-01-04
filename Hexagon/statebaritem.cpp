#include "statebaritem.h"

StateBarItem::StateBarItem(int maxValue, QGraphicsItem* parent) :
    QGraphicsItem(parent),
    currentValue(maxValue),
    maxValue(maxValue)
{}

void StateBarItem::setValue(int value)
{
    currentValue = qBound(0, value, maxValue); // Bound the value between 0 and maxValue
    update(); // draw the item again
}

void StateBarItem::setMaxValue(int maxValue)
{
    this->maxValue = maxValue;
    update();
}

QRectF StateBarItem::boundingRect() const
{
    return QRectF(0, 0, 300, 30); // size of the item
}

void StateBarItem::paint(QPainter* painter, const QStyleOptionGraphicsItem*, QWidget*)
{
    // draw background
    painter->setBrush(Qt::gray);
    painter->drawRect(0, 0, 300, 30);

    // draw bar
    int barWidth = (currentValue * 300) / maxValue;
    if (currentValue > 80)
        painter->setBrush(Qt::green);
    else if (currentValue > 50)
        painter->setBrush(Qt::yellow);
    else
        painter->setBrush(Qt::red);
    painter->drawRect(0, 0, barWidth, 30);
}
