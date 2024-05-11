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
    void scrollContentsBy(int dx, int dy);
    double ScaleSize=1.0;
    bool dragging=false;
    QPoint lastMousePosition;



};

#endif // ZOOMABLEGRAPHICSVIEW_H
