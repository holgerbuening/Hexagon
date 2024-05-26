#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QPixmap>
#include "zoomablegraphicsview.h"
#include "hexmap.h"
#include "unittype.h"
#include "unit.h"

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
    void onPushButtonNextTurnClicked();

private:
    Ui::MainWindow *ui;
    QGraphicsScene *scene;
    QGraphicsScene *sceneUnit;
    QGraphicsScene *sceneFlag;
    QGraphicsPixmapItem *itemUnit;
    QGraphicsPixmapItem *itemFlag;
    QPixmap pixmapNoUnit;
    QPixmap pixmapCountry1;
    QPixmap pixmapCountry2;
    HexMap *hexmap;
    std::vector<Unit>Units;
    bool move;
    int selectedUnitRow;
    int selectedUnitCol;
    Unit *selectedUnit;
    QString countryOnTheTurn;
    QString country1;
    QString country2;
    int round;

    void moveUnit(Unit* unit,int target_row, int target_col);
    void updateGraphicsView(QGraphicsScene *sceneUnit, QGraphicsView *view);
    void textBrowserFieldUpdate(QString row,QString col,QString fieldTypeText,QString movementCost);
    void textBrowserUnitUpdate (QString unitText, QString unitStatus, QString unitMovement);
    void drawMap();


};

#endif // MAINWINDOW_H
