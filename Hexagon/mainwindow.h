/*
 * This file is part of Hexagon
 *
 * Hexagon is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Hexagon is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Hexagon. If not, see <https://www.gnu.org/licenses/>.
 */


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
#include <memory>
#include <QLabel>

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
    void loadAMap();
    void saveAMap();
    void createNewMap();
    void startEditMapMode();
    void startGameMode();
    void stopGameMode();
    bool getGameModeStatus();
    void startCombat(Unit& attacker, Unit& defender);
    void moveUnit(Unit* unit,int target_row, int target_col);
    void isAnybodyDead();
    int getPlayerBalances(QString country);
    void setPlayerBalances(QString country, int balance);
    void GraphicsViewUpdate();
    void setDifficultyMultiplier(float multiplier);
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
    void saveMap(const QString& fileName);
    void loadMap(const QString& fileName);

private:
    std::unique_ptr<Ui::MainWindow> ui;
    std::unique_ptr<QMenuBar> menuBar;
    std::unique_ptr<QMenu> gameMenu;
    std::unique_ptr<QMenu> mapMenu;
    std::unique_ptr<QLabel> editModeLabel;
    std::unique_ptr<StartScreen> startScreen;
    std::unique_ptr<QAction> exitAction;
    std::unique_ptr<QAction> createNewMapAction;
    std::unique_ptr<QAction> gameSaveAction;
    std::unique_ptr<QAction> gameLoadAction;
    std::unique_ptr<QGraphicsScene> scene;
    std::unique_ptr<QGraphicsScene> sceneUnit;
    std::unique_ptr<QGraphicsScene> sceneFlag;
    std::unique_ptr<QGraphicsScene> sceneGearIcon;
    std::unique_ptr<QGraphicsPixmapItem> itemUnit;
    std::unique_ptr<QGraphicsPixmapItem> itemFlag;
    std::unique_ptr<QGraphicsPixmapItem> itemGearIcon;
    QPixmap pixmapNoUnit;
    QPixmap pixmapCountry1;
    QPixmap pixmapCountry2;
    QPixmap pixmapGearIcon;
    std::unique_ptr<QMediaPlayer> mediaPlayer;
    std::unique_ptr<QAudioOutput> audioOutput;
    std::unique_ptr<HexMap> hexmap;
    bool move; // is true during move process of a unit
    bool buyUnit; //is true during buying process of a unit
    bool healing; //is true during healing process of a unit
    bool editMapMode; //is true during editing the map
    bool gameMode; //is true during the game
    bool aiActivated;
    int selectedUnitRow=0;
    int selectedUnitCol=0;
    Unit* selectedUnit=nullptr; //selected unit for moving process
    UnitType::Type selectedUnitType=UnitType::Type::infantry; //type of Unit selected for buying process
    QString countryOnTheTurn;
    QString opponent;
    QString country1;
    QString country2;
    std::unordered_map<QString, int> playerBalances;
    int round;
    float difficultyMultiplier=1.0f;

    
    void updateGraphicsView(QGraphicsScene *sceneUnit, QGraphicsView *view);
    void textBrowserFieldUpdate(QString row,QString col,QString fieldTypeText,QString movementCost, QString fieldDefense);
    void textBrowserUnitUpdate (QString unitText, QString unitStatus, QString unitMovement, QString unitExperience, QString unitOffense, QString unitDefense, QString unitAttackRange);
    void drawMap();
    void setStartUnits();
    //void aiDetermineState(std::vector<Unit*> enemyUnits, std::vector<Unit*> objectives, std::vector<Unit*> ownUnits);
    //void aiPerformAction(Unit* unit, AIState state, std::vector<Unit*> enemyUnits, std::vector<Unit*> objectives);
    void showStartScreen();
    void startNewGame();
    void editMap(HexItem* selectedItem);
    void setGameVariables();
    void calculateCityIncome();
    void calculateIndustryIncome(); 
    void resizeEvent(QResizeEvent *event) override;
};

#endif // MAINWINDOW_H
