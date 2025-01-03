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


#include "zoomablegraphicsview.h"
#include <QCursor>




ZoomableGraphicsView::ZoomableGraphicsView(QWidget *parent)
    : QGraphicsView(parent)
{
    dragging=false;
}

ZoomableGraphicsView::~ZoomableGraphicsView() {
    // implement the destructor
}




void ZoomableGraphicsView::wheelEvent(QWheelEvent *event)
{
    // scale factor
    const double scaleFactor = 1.15;

    // anchor of the transformation
    setTransformationAnchor(QGraphicsView::AnchorUnderMouse);

    // zoom in or out
    if (event->angleDelta().y() > 0) {
        // zoom out
        if (ScaleSize < 1.0) {
            scale(scaleFactor, scaleFactor);
            ScaleSize *= scaleFactor;
        }
    } else {
        // zoom in
        if (ScaleSize > 0.15) {
            scale(1.0 / scaleFactor, 1.0 / scaleFactor);
            ScaleSize *= 1.0 / scaleFactor;
        }
    }

    // reset the anchor
    setTransformationAnchor(QGraphicsView::AnchorViewCenter);
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
        horizontalScrollBar()->setValue(horizontalScrollBar()->value() - delta.x());
        verticalScrollBar()->setValue(verticalScrollBar()->value() - delta.y());
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
        setCursor(Qt::ClosedHandCursor);  // change the cursor
    }
    QGraphicsView::mousePressEvent(event);
}
void ZoomableGraphicsView::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::RightButton)
    {
        dragging = false;
        setCursor(Qt::ArrowCursor);  // cursor back to normal
    }
    QGraphicsView::mouseReleaseEvent(event);
}


