#include "zoomablegraphicsview.h"



ZoomableGraphicsView::ZoomableGraphicsView(QWidget *parent)
    : QGraphicsView(parent) {}

ZoomableGraphicsView::ZoomableGraphicsView(QGraphicsScene *scene) : QGraphicsView(scene) {}

ZoomableGraphicsView::ZoomableGraphicsView(QGraphicsScene *scene, QWidget *parent)
    : QGraphicsView(scene, parent) {}


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
