#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsView>
#include <QGraphicsScene>
#include "zoomablegraphicsview.h"
#include "hexmap.h"


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onRadioButtonToggled(bool checked);
    void handleItemSelected(HexItem* selectedItem);

private:
    Ui::MainWindow *ui;
    QGraphicsScene *scene;
    HexMap *hexmap;
    void drawMap();


};

#endif // MAINWINDOW_H
