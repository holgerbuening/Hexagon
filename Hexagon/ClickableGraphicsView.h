#ifndef CLICKABLEGRAPHICSVIEW_H
#define CLICKABLEGRAPHICSVIEW_H

#include <QGraphicsView>
#include <QMouseEvent>

class ClickableGraphicsView : public QGraphicsView
{
    Q_OBJECT

public:
    explicit ClickableGraphicsView(QWidget *parent = nullptr);

signals:
    void clicked(); // Signal to notify about clicks

protected:
    void mousePressEvent(QMouseEvent *event) override;
};

#endif // CLICKABLEGRAPHICSVIEW_H
