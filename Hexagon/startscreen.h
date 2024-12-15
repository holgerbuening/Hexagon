#ifndef STARTSCREEN_H
#define STARTSCREEN_H

#include <QDialog>
#include <QGraphicsScene>
#include "ui_startscreen.h"

namespace Ui {
class StartScreen;
}

class StartScreen : public QDialog
{
    Q_OBJECT

public:
    explicit StartScreen(QWidget *parent = nullptr);
    ~StartScreen();

private slots:
    void on_pushButton_play_clicked();

private:
    Ui::StartScreenDialog *ui;
    QGraphicsScene *scene;
};

#endif // STARTSCREEN_H
