#ifndef STATEBARITEM_H
#define STATEBARITEM_H

#include <QGraphicsItem>
#include <QPainter>

class StateBarItem : public QGraphicsItem
{
public:
    explicit StateBarItem(int maxValue = 100, QGraphicsItem* parent = nullptr);

    void setValue(int value); // set the current value
    void setMaxValue(int maxValue); // set the maximum value

    QRectF boundingRect() const override;
    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) override;

private:
    int currentValue;
    int maxValue;
};

#endif // STATEBARITEM_H
