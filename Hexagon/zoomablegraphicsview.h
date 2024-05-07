#ifndef ZOOMABLEGRAPHICSVIEW_H
#define ZOOMABLEGRAPHICSVIEW_H

#include <QGraphicsView>
#include <QWheelEvent>
#include <QGraphicsScene>
#include <QApplication>
#include <QTimer>
#include <QMouseEvent>
#include <QScrollBar>


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
    void enterEvent(QEvent *event) override;
    void leaveEvent(QEvent *event) override;

private slots:
    void performAutoScroll();

private:
    double ScaleSize=1.0;
    QTimer *scrollTimer;
    int mouseX, mouseY;
    const int margin = 60;
    const int scrollSpeed = 10;



};

#endif // ZOOMABLEGRAPHICSVIEW_H
