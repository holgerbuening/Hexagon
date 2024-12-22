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


#ifndef ZOOMABLEGRAPHICSVIEW_H
#define ZOOMABLEGRAPHICSVIEW_H

#include <QGraphicsView>
#include <QWheelEvent>
#include <QGraphicsScene>
#include <QApplication>
#include <QTimer>
#include <QMouseEvent>
#include <QScrollBar>
#include <QCursor>


class ZoomableGraphicsView : public QGraphicsView
{
    //Q_OBJECT

public:
    ZoomableGraphicsView(QWidget *parent = nullptr);
    ~ZoomableGraphicsView();
    void setScaleSize (double size);


protected:
    void wheelEvent(QWheelEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event)override;
    void mousePressEvent(QMouseEvent *event)override;
    void mouseReleaseEvent(QMouseEvent *event)override;



private:
    double ScaleSize=1.0;
    bool dragging=false;
    QPoint lastMousePosition;



};

#endif // ZOOMABLEGRAPHICSVIEW_H
