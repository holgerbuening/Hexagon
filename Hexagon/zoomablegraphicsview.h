#ifndef ZOOMABLEGRAPHICSVIEW_H
#define ZOOMABLEGRAPHICSVIEW_H

#include <QGraphicsView>
#include <QWheelEvent>
#include <QGraphicsScene>
#include <QApplication>



class ZoomableGraphicsView : public QGraphicsView
{
public:
    ZoomableGraphicsView(QWidget *parent = nullptr);
    ZoomableGraphicsView(QGraphicsScene *scene);
    ZoomableGraphicsView(QGraphicsScene *scene, QWidget *parent = nullptr);
    void setScaleSize (double size);

private:
    double ScaleSize=1.0;

protected:
    void wheelEvent(QWheelEvent *event) override;
};

#endif // ZOOMABLEGRAPHICSVIEW_H
