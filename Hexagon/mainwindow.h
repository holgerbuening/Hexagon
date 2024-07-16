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
#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <QMediaPlayer>
#include <unordered_map>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    std::vector<Unit>Units;


private slots:
    void onRadioButtonToggled(bool checked);
    void handleItemSelected(HexItem* selectedItem);
    void onPushButtonNextTurnClicked();
    void onActionTriggered();
    void saveGame(const QString& fileName);
    void loadGame(const QString& fileName);

private:
    Ui::MainWindow *ui;
    QMenuBar *menuBar;
    QMenu *gameMenu;
    QMenu *mapMenu;
    QAction *exitAction;
    QAction *createNewMapAction;
    QAction *gameSaveAction;
    QAction *gameLoadAction;
    QGraphicsScene *scene;
    QGraphicsScene *sceneUnit;
    QGraphicsScene *sceneFlag;
    QGraphicsPixmapItem *itemUnit;
    QGraphicsPixmapItem *itemFlag;
    QPixmap pixmapNoUnit;
    QPixmap pixmapCountry1;
    QPixmap pixmapCountry2;
    QMediaPlayer *mediaPlayer;
    HexMap *hexmap;
    bool move;
    bool aiActivated;
    int selectedUnitRow;
    int selectedUnitCol;
    Unit *selectedUnit;
    QString countryOnTheTurn;
    QString opponent;
    QString country1;
    QString country2;
    std::unordered_map<QString, int> playerBalances;
    int round;

    void moveUnit(Unit* unit,int target_row, int target_col);
    void updateGraphicsView(QGraphicsScene *sceneUnit, QGraphicsView *view);
    void textBrowserFieldUpdate(QString row,QString col,QString fieldTypeText,QString movementCost, QString fieldDefense);
    void textBrowserUnitUpdate (QString unitText, QString unitStatus, QString unitMovement, QString unitExperience, QString unitOffense, QString unitDefense, QString unitAttackRange);
    void drawMap();
    void setStartUnits();
    void startNewGame();
    void startCombat(Unit& attacker, Unit& defender);
    void isAnybodyDead();
    void aiDetermineState(std::vector<Unit*> enemyUnits, std::vector<Unit*> objectives, std::vector<Unit*> ownUnits);
    void aiPerformAction(Unit* unit, AIState state, std::vector<Unit*> enemyUnits, std::vector<Unit*> objectives);
};

#endif // MAINWINDOW_H
