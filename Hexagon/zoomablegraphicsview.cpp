#include "zoomablegraphicsview.h"
#include <QCursor>




ZoomableGraphicsView::ZoomableGraphicsView(QWidget *parent)
    : QGraphicsView(parent)
{

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



void ZoomableGraphicsView::setScaleSize(double size)
{
    scale(size, size);
    ScaleSize=size;
}



void ZoomableGraphicsView::mouseMoveEvent(QMouseEvent *event)
{

    if (dragging)
    {
        QPointF delta = event->pos() - lastMousePosition;
        scrollContentsBy(delta.x(), delta.y());
        lastMousePosition = event->pos();
    }
    QGraphicsView::mouseMoveEvent(event);
}
void ZoomableGraphicsView::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::RightButton)
    {
        dragging = true;
        lastMousePosition = event->pos();
        setCursor(Qt::ClosedHandCursor);  // Optional: Ändere den Mauszeiger
    }
    QGraphicsView::mousePressEvent(event);
}
void ZoomableGraphicsView::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::RightButton)
    {
        dragging = false;
        setCursor(Qt::ArrowCursor);  // Optional: Setze den Mauszeiger zurück
    }
    QGraphicsView::mouseReleaseEvent(event);
}
void ZoomableGraphicsView::scrollContentsBy(int dx, int dy)
{
    horizontalScrollBar()->setValue(horizontalScrollBar()->value() - dx);
    verticalScrollBar()->setValue(verticalScrollBar()->value() - dy);
}

