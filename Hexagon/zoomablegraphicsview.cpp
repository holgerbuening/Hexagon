#include "zoomablegraphicsview.h"




ZoomableGraphicsView::ZoomableGraphicsView(QWidget *parent)
    : QGraphicsView(parent), scrollTimer(new QTimer(this))
{
    connect(scrollTimer, &QTimer::timeout, this, &ZoomableGraphicsView::performAutoScroll);
    //scrollTimer->start(30);
    scrollTimer->stop();
}

ZoomableGraphicsView::~ZoomableGraphicsView() {
    // Implementierung, selbst wenn sie leer ist
}




void ZoomableGraphicsView::wheelEvent(QWheelEvent *event)
{
        // Skalierungsfaktor festlegen
        const double scaleFactor = 1.15;

        // Herauszoomen
        if (event->angleDelta().y() > 0) {
            // Vergrößern
            if (ScaleSize<1.0)
            {
            scale(scaleFactor, scaleFactor);
            ScaleSize=ScaleSize*scaleFactor;
            }
        } else {
            // Verkleinern
            if (ScaleSize>0.15)
            {
            scale(1.0 / scaleFactor, 1.0 / scaleFactor);
            ScaleSize=ScaleSize*1.0/ scaleFactor;
            }
        }
}

void ZoomableGraphicsView::performAutoScroll() {
   if (mouseX < margin) {
        horizontalScrollBar()->setValue(horizontalScrollBar()->value() - scrollSpeed);
    } else if (mouseX > width() - margin) {
        horizontalScrollBar()->setValue(horizontalScrollBar()->value() + scrollSpeed);
    }

    if (mouseY < margin) {
        verticalScrollBar()->setValue(verticalScrollBar()->value() - scrollSpeed);
    } else if (mouseY > height() - margin) {
        verticalScrollBar()->setValue(verticalScrollBar()->value() + scrollSpeed);
    }
}

void ZoomableGraphicsView::setScaleSize(double size)
{
    scale(size, size);
    ScaleSize=size;
}

void ZoomableGraphicsView::enterEvent(QEvent *event) {
    scrollTimer->start(30);
}

void ZoomableGraphicsView::leaveEvent(QEvent *event) {
    scrollTimer->stop();
}

void ZoomableGraphicsView::mouseMoveEvent(QMouseEvent *event)
{
    mouseX = event->pos().x();
    mouseY = event->pos().y();
}
