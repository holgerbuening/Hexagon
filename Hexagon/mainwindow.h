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
#include <QAudioOutput>
#include <unordered_map>
#include "startscreen.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    
    void loadAGame();
    void saveAGame();
    void createNewMap();
    void startEditMapMode();
    std::vector<Unit>Units;

protected:
    void keyPressEvent(QKeyEvent *event) override; //Override the keyPressEvent function to handle key events

private slots:
    void onRadioButtonToggled(bool checked);
    void handleItemSelected(HexItem* selectedItem);
    void onPushButtonNextTurnClicked();
    void onActionTriggered();
    void saveGame(const QString& fileName);
    void loadGame(const QString& fileName);
    void onGearIconClicked();

private:
    Ui::MainWindow *ui;
    QMenuBar *menuBar;
    QMenu *gameMenu;
    QMenu *mapMenu;
    StartScreen *startScreen;
    QAction *exitAction;
    QAction *createNewMapAction;
    QAction *gameSaveAction;
    QAction *gameLoadAction;
    QGraphicsScene *scene;
    QGraphicsScene *sceneUnit;
    QGraphicsScene *sceneFlag;
    QGraphicsScene *sceneGearIcon;
    QGraphicsPixmapItem *itemUnit;
    QGraphicsPixmapItem *itemFlag;
    QGraphicsPixmapItem *itemGearIcon;
    QPixmap pixmapNoUnit;
    QPixmap pixmapCountry1;
    QPixmap pixmapCountry2;
    QPixmap pixmapGearIcon;
    QMediaPlayer *mediaPlayer;
    QAudioOutput *audioOutput;
    HexMap *hexmap;
    bool move; // is true during move process of a unit
    bool buyUnit; //is true during buying process of a unit
    bool healing; //is true during healing process of a unit
    bool editMapMode; //is true during editing the map
    bool aiActivated;
    int selectedUnitRow;
    int selectedUnitCol;
    Unit *selectedUnit;
    UnitType::Type selectedUnitType; //type of Unit selected for buying process
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
    void startCombat(Unit& attacker, Unit& defender);
    void isAnybodyDead();
    void aiDetermineState(std::vector<Unit*> enemyUnits, std::vector<Unit*> objectives, std::vector<Unit*> ownUnits);
    void aiPerformAction(Unit* unit, AIState state, std::vector<Unit*> enemyUnits, std::vector<Unit*> objectives);
    void showStartScreen();
    void startNewGame();
    void editMap(HexItem* selectedItem);
};

#endif // MAINWINDOW_H
