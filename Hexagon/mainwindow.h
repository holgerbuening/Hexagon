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
    HexMap *hexmap;
    void drawMap();
    std::vector<Unit>Units;
    bool move;
    int selectedUnitRow;
    int selectedUnitCol;
    Unit *selectedUnit;
    void moveUnit(Unit* unit,int target_row, int target_col);



};

#endif // MAINWINDOW_H
