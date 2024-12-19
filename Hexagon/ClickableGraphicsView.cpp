#include "ClickableGraphicsView.h"

ClickableGraphicsView::ClickableGraphicsView(QWidget *parent)
    : QGraphicsView(parent)
{
}

// Emit the clicked signal when the mouse is pressed
void ClickableGraphicsView::mousePressEvent(QMouseEvent *event)
{
    QGraphicsView::mousePressEvent(event);
    emit clicked();
}
