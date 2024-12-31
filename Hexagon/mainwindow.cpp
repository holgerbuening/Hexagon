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


#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "zoomablegraphicsview.h"
#include "ClickableGraphicsView.h"
#include "fieldtype.h"
#include "hexmap.h"
#include "hexitem.h"
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QMessageBox>
#include "unittype.h"
#include "unit.h"
#include "combatdialog.h"
#include "headquarterdialog.h"
#include "startscreen.h"
#include "mapsizedialog.h"
#include <random>
#include <iostream>
#include <QFile>
#include <QDataStream>
#include <QFileDialog>
#include <QTimer>
#include <QKeyEvent>
#include <QInputDialog>
#include "customdialog.h"
#include "aimanager.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(std::make_unique<Ui::MainWindow>()),
    scene(std::make_unique<QGraphicsScene>(this)),
    sceneUnit(std::make_unique<QGraphicsScene>(this)),
    sceneFlag(std::make_unique<QGraphicsScene>(this)),
    sceneGearIcon(std::make_unique<QGraphicsScene>(this)),
    hexmap(std::make_unique<HexMap>(20,12,std::move(scene))),
    Units(),
    mediaPlayer(std::make_unique<QMediaPlayer>(this)),
    audioOutput(std::make_unique<QAudioOutput>(this)),
    selectedUnit(nullptr)
{
    // initial settings
    ui->setupUi(this);
    FieldType::loadPixmaps();
    UnitType::loadUnits();
    
    // Hinzufügen des Pfades zu den Plugins
    QCoreApplication::addLibraryPath(QDir::currentPath() + "/plugins");


    // set audio output
    mediaPlayer->setAudioOutput(audioOutput.get());


    // create menu
    menuBar = std::make_unique<QMenuBar>(this);
    setMenuBar(menuBar.get());
    gameMenu = std::make_unique<QMenu>("Game", this);
    exitAction = std::make_unique<QAction>("Exit", this);
    gameSaveAction = std::make_unique<QAction>("Save Game",this);
    gameLoadAction = std::make_unique<QAction>("Load Game",this);
    gameMenu->addAction(gameSaveAction.get());
    gameMenu->addAction(gameLoadAction.get());
    gameMenu->addAction(exitAction.get());
    menuBar->addMenu(gameMenu.get());
    mapMenu = std::make_unique<QMenu>("Map",this);
    createNewMapAction = std::make_unique<QAction>("Create new map",this);
    mapMenu->addAction(createNewMapAction.get());
    menuBar->addMenu(mapMenu.get());


    //Signal - Slot Connections
    connect(ui->radioButton, &QRadioButton::toggled, this, &MainWindow::onRadioButtonToggled);
    connect(ui->pushButtonNextTurn, &QPushButton::clicked, this, &MainWindow::onPushButtonNextTurnClicked);
    connect(exitAction.get(), &QAction::triggered, this, &MainWindow::onActionTriggered);
    connect(createNewMapAction.get(), &QAction::triggered, this, &MainWindow::onActionTriggered);
    connect(gameSaveAction.get(), &QAction::triggered, this,&MainWindow::onActionTriggered);
    connect(gameLoadAction.get(), &QAction::triggered, this,&MainWindow::onActionTriggered);
    connect(ui->graphicsView_gearIcon, &ClickableGraphicsView::clicked, this, &MainWindow::onGearIconClicked);

    //create and draw map
    hexmap->createRandomMap();
    drawMap();

    //set game variables and flags
    country1="Lupony";
    country2="Ursony";
    countryOnTheTurn=country1;
    opponent=country2;
    pixmapCountry1= QPixmap(":/Images/flag_lupony.png");
    pixmapCountry2= QPixmap(":/Images/flag_ursony.png");
    itemFlag = std::make_unique<QGraphicsPixmapItem>(pixmapCountry1);
    editMapMode=false;
    gameMode=false;
    move=false;
    buyUnit=false;
    healing=false;
    aiActivated=true;
    round=1;
    playerBalances[country1]=100;
    playerBalances[country2]=100;
    setGameVariables();
    
    

    /*//create Units
    setStartUnits();
    hexmap->drawUnits(&Units);*/

    
    //prepare main View
    ui->graphicsView->setScene(hexmap->getScene());
    ui->graphicsView->setRenderHint(QPainter::Antialiasing);
    ui->graphicsView->setBackgroundBrush(Qt::lightGray);
    ui->graphicsView->show();

    //prepare sidebar Views
    ui->graphicsView_gearIcon->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->graphicsView_gearIcon->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    pixmapGearIcon = QPixmap(":/Images/gear_icon.png");
    itemGearIcon = std::make_unique<QGraphicsPixmapItem>(pixmapGearIcon);
    sceneGearIcon->addItem(itemGearIcon.get());
    ui->graphicsView_gearIcon->setScene(sceneGearIcon.get());
    ui->graphicsView_gearIcon->show();
    pixmapNoUnit =  QPixmap(":/Images/noUnit.png");
    itemUnit = std::make_unique<QGraphicsPixmapItem>(pixmapNoUnit);
    ui->graphicsViewUnit->setScene(sceneUnit.get());
    sceneUnit->addItem(itemUnit.get());
    ui->graphicsViewUnit->fitInView(itemUnit.get(), Qt::KeepAspectRatio);
    ui->graphicsViewUnit->setRenderHint(QPainter::Antialiasing);
    ui->graphicsViewUnit->show();
    ui->graphicsViewFlag->setScene(sceneFlag.get());
    sceneFlag->addItem(itemFlag.get());
    ui->graphicsViewFlag->show();
    ui->lcdNumber->display(playerBalances[countryOnTheTurn]);

    //show startscreen after initializaion of MainWindow
    QTimer::singleShot(0, this,&MainWindow::showStartScreen);

}// end of MainWindow constructor

MainWindow::~MainWindow()
{
    qDebug() << "MainWindow destructor:";
    qDebug() << "ui is Smart Pointer and will be deleted automatically";
    qDebug() << "hexmap is Smart Pointer and will be deleted automatically";
    qDebug() << "menuBar is Smart Pointer and will be deleted automatically";
    qDebug() << "gameMenu is Smart Pointer and will be deleted automatically";
    qDebug() << "mapMenu is Smart Pointer and will be deleted automatically"; 
    qDebug() << "gameSaveAction is Smart Pointer and will be deleted automatically";
    qDebug() << "gameLoadAction is Smart Pointer and will be deleted automatically";    
    qDebug() << "exitAction is Smart Pointer and will be deleted automatically";
    qDebug() << "createNewMapAction is Smart Pointer and will be deleted automatically";
    qDebug() << "scene is Smart Pointer and will be deleted automatically";
    qDebug() << "sceneUnit is Smart Pointer and will be deleted automatically";
    qDebug() << "sceneFlag is Smart Pointer and will be deleted automatically";
    qDebug() << "sceneGearIcon is Smart Pointer and will be deleted automatically";
    qDebug() << "itemFlag is Smart Pointer and will be deleted automatically";
    qDebug() << "itemUnit is Smart Pointer and will be deleted automatically";
    qDebug() << "itemGearIcon is Smart Pointer and will be deleted automatically";
    qDebug() << "mediaPlayer is Smart Pointer and will be deleted automatically";
    qDebug() << "audioOutput is Smart Pointer and will be deleted automatically";
    
    if (selectedUnit)
    {
        qDebug() << "selectedUnit is not null and will be deleted";
        selectedUnit = nullptr;
    }
    qDebug() << "startScreen is Smart Pointer and will be deleted automatically";
    qDebug() << "MainWindow destructor finished";
}

void MainWindow::setStartUnits()
{
bool found=false;
Units.clear();
int rowBase1;
int colBase1;
int maxAttempts=hexmap->getHeight()*hexmap->getWidth();
int attempts=0;

    //find base for country1
    while (!found && attempts<maxAttempts)
    {
        attempts++;
        int row = rand()%hexmap->getHeight();
        int col = rand()% (hexmap->getWidth()/3);
        if (FieldType::getTerritory(hexmap->getHex(row,col).getFieldType())==0)
        {
            std::vector<Hex> neighbours=hexmap->getNeighborsSameTerritory(hexmap->getHex(row,col),0);
            size_t count = neighbours.size();
            if (count > 2)
            {
                std::random_device rd;
                std::mt19937 g(rd());
                std::shuffle(neighbours.begin(), neighbours.end(), g );
                Unit inf1(UnitType::infantry, neighbours.at(0).getRow(), neighbours.at(0).getCol(), country1);
                Unit inf2(UnitType::infantry,neighbours.at(1).getRow(),neighbours.at(1).getCol(),country1);
                Unit mach(UnitType::machineGun,neighbours.at(2).getRow(),neighbours.at(2).getCol(),country1);
                Unit Base(UnitType::militarybase,row,col,country1);
                Units.push_back(inf1);
                Units.push_back(inf2);
                Units.push_back(mach);
                Units.push_back(Base);
                rowBase1=row;
                colBase1=col;
                found=true;
            }

        }
    }
    if (!found)
    {
        QMessageBox::critical(this, "Error", "Failed to place starting units for Country 1 after maximum attempts. Create a new map!");
        return;
    }

    //find base for country2
    found=false;
    attempts=0;
    while (!found && attempts<maxAttempts)
    {
        attempts++;
        int row = rand()%hexmap->getHeight();
        int col = (rand()% (hexmap->getWidth()/3))+hexmap->getWidth()/3*2;
        if (FieldType::getTerritory(hexmap->getHex(row,col).getFieldType())==0)
        {
            //check if is possible to move to base of country1 via land
            if (hexmap->calculateMovementCost(row,col,rowBase1,colBase1,0)!=-1)
            {
                std::vector<Hex> neighbours=hexmap->getNeighborsSameTerritory(hexmap->getHex(row,col),0);
                size_t count = neighbours.size();
                if (count > 2)
                {
                    std::random_device rd;
                    std::mt19937 g(rd());
                    std::shuffle(neighbours.begin(), neighbours.end(), g );
                    Unit inf1(UnitType::infantry, neighbours.at(0).getRow(), neighbours.at(0).getCol(), country2);
                    Unit inf2(UnitType::infantry,neighbours.at(1).getRow(),neighbours.at(1).getCol(),country2);
                    Unit mach(UnitType::machineGun,neighbours.at(2).getRow(),neighbours.at(2).getCol(),country2);
                    Unit Base(UnitType::militarybase,row,col,country2);
                    Units.push_back(inf1);
                    Units.push_back(inf2);
                    Units.push_back(mach);
                    Units.push_back(Base);
                    found=true;
                }
            }
        }
    }
    if (!found)
    {
        QMessageBox::critical(this, "Error", "Failed to place starting units for Country 2 after maximum attempts. Create a new map!");
        return;
    }
}// end of setStartUnits

void MainWindow::drawMap()
{
    if (hexmap->hexItems.empty())
    {
        for (int row = 0; row < hexmap->getHeight(); ++row)
        {
            for (int col = 0; col < hexmap->getWidth(); ++col)
            {
                const Hex& hex = hexmap->getHex(row, col);
                int x = col * hexmap->getXOffset();
                int y = row * hexmap->getYOffset() + (col % 2) * (hexmap->getHexHeight() / 2);
                //std::unique_ptr<HexItem> item = std::make_unique<HexItem>(FieldType::getPixmap(hex.getFieldType()), nullptr, row,col);
                HexItem* item = new HexItem(FieldType::getPixmap(hex.getFieldType()), nullptr, row,col);
                item->setPos(x, y);
                //hexmap->hexItems.push_back(item.get());
                hexmap->hexItems.push_back(item);
                connect(item->getSignalHelper(), &HexItemSignalHelper::itemSelected, this, &MainWindow::handleItemSelected);
            }
        }
    }
    hexmap->addHexItemsToScene();
}

void MainWindow::onRadioButtonToggled(bool checked)
{
    if (checked)
        {
        hexmap->drawGrid(); // Raster zeichnen, wenn der Button ausgewählt ist
        }
    else
        {
        hexmap->removeGridItemsFromScene();
    }
}

//Main Handling of Key Events
void MainWindow::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Escape)
    {
        showStartScreen();
    }
    else
    {
        QMainWindow::keyPressEvent(event);
    }
}

//Main Handling of Clicks on the map
void MainWindow::handleItemSelected(HexItem* selectedItem)
{
    //set initial variables for the move process

    Unit *selectedUnitThisClick=nullptr;
    bool unit_clicked=false;
    int distance=0;
    QString unitText("no unit");
    QString unitStatus("no unit");
    QString unitMovement("no unit");
    QString unitExperience("no unit");
    QString unitOffense("no unit");
    QString unitDefense("no unit");
    QString unitAttackRange("no unit");
    int row= selectedItem->getrow();
    int col= selectedItem->getcol();
    int territory;
    const Hex& hex = hexmap->getHex(row, col);
    QString fieldTypeText=hex.getFieldTypeText();
    int movementCost = hex.getMovementCost();
    int fieldDefense = hex.getDefense();

    //check if in Edit Map Mode
    if (editMapMode)
    {
        selectedItem->deleteOverlayItem();
        editMap(selectedItem);
    }
    else
    {
        //check if an unit was clicked
        for (std::vector<Unit>::iterator it = Units.begin(); it!= Units.end(); ++it)
        {
            if (it->getCol()==col && it->getRow()==row) //clicked on an unit
            {
                unit_clicked=true;
                selectedUnitThisClick = &(*it);
                unitText =it->getUnitTypeText();
                territory=it->getTerritory();
                unitStatus = QString::number(it->getCurrentState());
                unitExperience = QString::number(it->getExperience());
                unitOffense=QString::number(it->getOffense());
                unitDefense=QString::number(it->getDefense());
                unitAttackRange=QString::number(it->getAttackRange());
                distance=it->getRemainingMovementPoints();
                unitMovement=QString::number(distance);
            }
        }

        

        //clicked on headquarter -> start "buy a unit"
        if (unit_clicked && selectedUnitThisClick->getCountry()==countryOnTheTurn && !selectedUnitThisClick->getActed() && unitText=="Headquarter")// clicked on Headquarter -> start unit sale
        {
            move=false;
            healing=false;

            hexmap->clearActiveMoveOverlay();
            hexmap->clearActiveAttackOverlay();
            hexmap->setActiveOverlay(selectedItem->overlayItem);

            //append all unit types except military base
            QList<UnitType::Type> unitTypes;
            for (int i = 0; i < static_cast<int>(UnitType::Count); ++i) 
            {
                UnitType::Type type = static_cast<UnitType::Type>(i);
                if (type != UnitType::militarybase)
                {
                    unitTypes.append(type);
                }
            }

            HeadquarterDialog hqdialog(playerBalances[countryOnTheTurn], this);
            hqdialog.populateUnitList(unitTypes);

            if (hqdialog.exec()==QDialog::Accepted) // first step of buying process
            {
                buyUnit=true;
                hexmap->drawActiveMoveOverlay(row,col,4,territory, &Units);
                selectedUnitType = hqdialog.getSelectedUnitType();
                selectedUnit=selectedUnitThisClick; //mark the selected Unit for the move process
                selectedUnitCol=col; //mark the actual position of the selected Unit
                selectedUnitRow=row;
                //hexmap->clearActiveOverlay();
            }
            else // no buying process
            {
                selectedUnit=nullptr; //mark the selected Unit for the move process
                //selectedUnitCol=col; //mark the actual position of the selected Unit
                //selectedUnitRow=row;
                hexmap->clearActiveOverlay();
            }


        }

        //clicked on an opponent unit during buying process ->end buying process information only
        else if (buyUnit && unit_clicked && selectedUnitThisClick->getCountry()!=countryOnTheTurn)
        {
            hexmap->setActiveOverlay(selectedItem->overlayItem);
            hexmap->clearActiveOverlay();
            selectedUnit=nullptr;
            hexmap->clearActiveMoveOverlay();
            hexmap->clearActiveAttackOverlay();
            buyUnit=false;
        }

        //clicked on a unit but no unit selected so far -> first step of movement/attack
        else if (!move && unit_clicked && selectedUnitThisClick->getCountry()==countryOnTheTurn && !selectedUnitThisClick->getActed()&& unitText!="Medic")
        {
            move=true;
            if(buyUnit)
            {
                hexmap->clearActiveMoveOverlay();
                hexmap->clearActiveAttackOverlay();
                buyUnit=false;
            }
            if(healing)
            {
                hexmap->clearActiveMoveOverlay();
                hexmap->clearActiveAttackOverlay();
                healing=false;
            }

            hexmap->setActiveOverlay(selectedItem->overlayItem);
            hexmap->drawActiveMoveOverlay(row,col,distance,territory, &Units);
            hexmap->drawActiveAttackOverlay(row,col,selectedUnitThisClick->getAttackRange(),opponent,&Units);
            selectedUnit=selectedUnitThisClick; //mark the selected Unit for the move process
            selectedUnitCol=col; //mark the actual position of the selected Unit
            selectedUnitRow=row;
        }

        //clicked on a  medic unit but no unit selected so far -> first step of healing
        else if (!move && unit_clicked && selectedUnitThisClick->getCountry()==countryOnTheTurn && !selectedUnitThisClick->getActed() && unitText=="Medic")
        {
            //qDebug() << "Healing Process!" << '/n';
            move=true;
            healing=true;
            if(buyUnit)
            {
                hexmap->clearActiveMoveOverlay();
                hexmap->clearActiveAttackOverlay();
                buyUnit=false;
            }
            hexmap->setActiveOverlay(selectedItem->overlayItem);
            hexmap->drawActiveMoveOverlay(row,col,distance,territory, &Units);
            hexmap->drawActiveAttackOverlay(row,col,selectedUnitThisClick->getAttackRange(),countryOnTheTurn,&Units);
            selectedUnit=selectedUnitThisClick; //mark the selected Unit for the move process
            selectedUnitCol=col; //mark the actual position of the selected Unit
            selectedUnitRow=row;
        }


        //clicked on a unit unit acted already -> no action only information
        else if (!move && !healing && unit_clicked && selectedUnitThisClick->getCountry()==countryOnTheTurn && selectedUnitThisClick->getActed())
        {
            hexmap->setActiveOverlay(selectedItem->overlayItem);
            selectedUnit=selectedUnitThisClick; //mark the selected Unit for the move process
            selectedUnitCol=col; //mark the actual position of the selected Unit
            selectedUnitRow=row;
            buyUnit=false;
        }


        //second selection of the same unit -> Deselection
        else if ((move||buyUnit||healing) && unit_clicked && selectedUnitCol==col && selectedUnitRow==row && selectedUnitThisClick->getCountry()==countryOnTheTurn)//second selection om the same unit -> Deselection
        {
            hexmap->clearActiveMoveOverlay();
            hexmap->clearActiveAttackOverlay();
            hexmap->setActiveOverlay(selectedItem->overlayItem);
            move=false;
            buyUnit=false;
            healing=false;
            selectedUnit=nullptr;
        }

        //different unit selected during move process -> no Movement -> new Unit is selected for Move process or will be healed
        else if ((move||buyUnit||healing) && unit_clicked && selectedUnitThisClick->getCountry()==countryOnTheTurn && !selectedUnitThisClick->getActed())
        {
            if (healing && hexmap->distance(selectedUnit->getRow(),selectedUnit->getCol(),selectedUnitThisClick->getRow(),selectedUnitThisClick->getCol())<=selectedUnit->getAttackRange()) //start healing process
            {
                hexmap->setActiveOverlay(selectedItem->overlayItem);
                hexmap->clearActiveOverlay();

                hexmap->clearActiveMoveOverlay();
                hexmap->clearActiveAttackOverlay();
                move=false;
                healing=false;
                if (selectedUnitThisClick->getCurrentState()<100)
                {
                    int new_state = selectedUnitThisClick->getCurrentState();
                    new_state+=50;
                    if (new_state>100) new_state=100;
                    selectedUnitThisClick->setCurrentState(new_state);
                    //QMessageBox::information(this,"Unit healed","The new state of this unit is: " + QString::number(new_state));
                    CustomDialog::showDialogWithOneButton("The unit was healed! The new state of this unit is: " + QString::number(new_state),"OK",":/Images/dialogbackground2",this);
                    selectedUnit->setActed();
                }
                else
                { //QMessageBox::information(this,"Unit healed","The unit was already in a perfect state!");
                CustomDialog::showDialogWithOneButton("The unit was already in a perfect state!","OK",":/Images/dialogbackground2",this);
                }
                selectedUnit=nullptr;
            }
            else
            {
                hexmap->clearActiveMoveOverlay();
                hexmap->clearActiveAttackOverlay();
                selectedUnit=selectedUnitThisClick;
                selectedUnitRow=row;
                selectedUnitCol=col;
                move=true;
                buyUnit=false;
                hexmap->setActiveOverlay(selectedItem->overlayItem);
                hexmap->drawActiveMoveOverlay(row,col,distance,territory, &Units);
                if (unitText!="Medic")
                {
                    hexmap->drawActiveAttackOverlay(row,col,selectedUnitThisClick->getAttackRange(),opponent,&Units);
                }
                else
                {
                    hexmap->drawActiveAttackOverlay(row,col,selectedUnitThisClick->getAttackRange(),countryOnTheTurn,&Units);
                    healing=true;
                }
            }
        }

        //different unit selected during move process -> no Movement -> new Unit has no action points left -> information only
        else if ((move||buyUnit||healing) && unit_clicked && selectedUnitThisClick->getCountry()==countryOnTheTurn && selectedUnitThisClick->getActed())
        {
            hexmap->clearActiveMoveOverlay();
            hexmap->clearActiveAttackOverlay();
            selectedUnit=selectedUnitThisClick;
            selectedUnitRow=row;
            selectedUnitCol=col;
            move=false;
            buyUnit=false;
            healing=false;
            hexmap->setActiveOverlay(selectedItem->overlayItem);
        }

        //opponent unit selected during move process -> ATTACK
        else if (move && unit_clicked && selectedUnitThisClick->getCountry()!=countryOnTheTurn
                && hexmap->distance(selectedUnit->getRow(),selectedUnit->getCol(),selectedUnitThisClick->getRow(),selectedUnitThisClick->getCol())<=selectedUnit->getAttackRange())
        {

            hexmap->setActiveOverlay(selectedItem->overlayItem);
            hexmap->clearActiveOverlay();

            hexmap->clearActiveMoveOverlay();
            hexmap->clearActiveAttackOverlay();
            move=false;
            if (selectedUnit!=nullptr)
            {
            Unit& defender=*selectedUnitThisClick;
            Unit& attacker = *selectedUnit;
            startCombat(attacker, defender);
            }
            selectedUnit=nullptr;
            //check if any unit was killed
            isAnybodyDead();
        }

        //opponent unit selected during move process but out of Attack Range -> clear overlay
        else if (move && unit_clicked && selectedUnitThisClick->getCountry()!=countryOnTheTurn
                && hexmap->distance(selectedUnit->getRow(),selectedUnit->getCol(),selectedUnitThisClick->getRow(),selectedUnitThisClick->getCol())>selectedUnit->getAttackRange())
        {
            hexmap->setActiveOverlay(selectedItem->overlayItem);
            hexmap->clearActiveOverlay();
            selectedUnit=nullptr;
            hexmap->clearActiveMoveOverlay();
            hexmap->clearActiveAttackOverlay();
            move=false;
        }

        //opponent unit selected while NOT in move process -> clear overlay
        else if (!move && unit_clicked && selectedUnitThisClick->getCountry()!=countryOnTheTurn)
        {
            hexmap->setActiveOverlay(selectedItem->overlayItem);
            hexmap->clearActiveOverlay();
            selectedUnit=nullptr;
        }

        //clicked onto an empty field
        else if (!unit_clicked)
        {

            //clicked onto an empty field but no unit selected so far -> not in move process ->plain field informaiton
            if (move==false && buyUnit==false && healing==false)
            {
            hexmap->setActiveOverlay(selectedItem->overlayItem);
            selectedUnit=nullptr;
            }

            // clicked onto an empty field and a unit was selected before (possible move process)
            else if (move)
            {

                //clicked onto an empty field and a unit was selected before, the field is within this units range ->move Unit >>MOVE<<
                if (hexmap->calculateMovementCost(selectedUnitRow,selectedUnitCol,row,col,selectedUnit->getTerritory(),&Units)<=selectedUnit->getRemainingMovementPoints()
                        && hexmap->calculateMovementCost(selectedUnitRow,selectedUnitCol,row,col,selectedUnit->getTerritory(),&Units)!=-1
                        && selectedUnit->getTerritory()==FieldType::getTerritory(hexmap->getHex(row,col).getFieldType()))
                {
                    moveUnit(selectedUnit,row,col);
                    unitText=selectedUnit->getUnitTypeText();
                    unitStatus=QString::number(selectedUnit->getCurrentState());
                    unitMovement=QString::number(selectedUnit->getRemainingMovementPoints());
                    unitExperience=QString::number(selectedUnit->getExperience());
                    unitOffense=QString::number(selectedUnit->getOffense());
                    unitDefense=QString::number(selectedUnit->getDefense());
                    move=false;
                    hexmap->clearActiveMoveOverlay();
                    hexmap->clearActiveAttackOverlay();
                    hexmap->setActiveOverlay(selectedItem->overlayItem);

                }

                //clicked onto an empty field and a unit was selected before, the field is NOT in this units range ->no move but plain field information
                else
                {
                hexmap->clearActiveMoveOverlay();
                hexmap->clearActiveAttackOverlay();
                hexmap->setActiveOverlay(selectedItem->overlayItem);
                selectedUnit=nullptr;
                move=false;
                }
            }//end of clicked on empty field while in move process

            //clicked on empty field and during buying process
            else if (buyUnit)
            {
                //clicked onto an empty field and a unit was selected before, the field is within headquaterrange ->place new Unit
                if (hexmap->calculateMovementCost(selectedUnitRow,selectedUnitCol,row,col,selectedUnit->getTerritory(),&Units)<=4
                    && hexmap->calculateMovementCost(selectedUnitRow,selectedUnitCol,row,col,selectedUnit->getTerritory(),&Units)!=-1
                    && selectedUnit->getTerritory()==FieldType::getTerritory(hexmap->getHex(row,col).getFieldType()))
                {

                    //qDebug()<<"Selected unit type: " << UnitType::getName(selectedUnitType);
                    Unit newUnit(selectedUnitType,row,col,countryOnTheTurn);
                    Units.push_back(newUnit);
                    selectedUnit=&Units.back();
                    playerBalances[countryOnTheTurn]-= UnitType::getPrice(selectedUnit->getType());
                    selectedUnit->setActed();
                    buyUnit=false;
                    hexmap->clearUnits();
                    hexmap->drawUnits(&Units);
                    ui->graphicsView->update();
                    hexmap->clearActiveMoveOverlay();
                    hexmap->clearActiveAttackOverlay();
                    hexmap->setActiveOverlay(selectedItem->overlayItem);

                    unitText=selectedUnit->getUnitTypeText();
                    unitStatus=QString::number(selectedUnit->getCurrentState());
                    unitMovement=QString::number(selectedUnit->getRemainingMovementPoints());
                    unitExperience=QString::number(selectedUnit->getExperience());
                    unitOffense=QString::number(selectedUnit->getOffense());
                    unitDefense=QString::number(selectedUnit->getDefense());
                }

                //clicked onto an empty field and a unit was selected before, the field is NOT in this units range ->no move but plain field information
                else
                {
                    hexmap->clearActiveMoveOverlay();
                    hexmap->clearActiveAttackOverlay();
                    hexmap->setActiveOverlay(selectedItem->overlayItem);
                    selectedUnit=nullptr;
                    buyUnit=false;
                }
            }//end of clicked on empty field while in buying process




        }// end of clicked on empty field

        //something happended we did not foresee
        else
        {
            hexmap->clearActiveMoveOverlay();
            hexmap->clearActiveAttackOverlay();
            hexmap->setActiveOverlay(selectedItem->overlayItem);
            selectedUnit=nullptr;
            move=false;
            buyUnit=false;
            healing=false;
            unitText="Unforeseen Event";
        }

        textBrowserFieldUpdate(QString::number(row),QString::number(col),fieldTypeText,QString::number(movementCost), QString::number(fieldDefense));
        textBrowserUnitUpdate(unitText,unitStatus,unitMovement, unitExperience, unitOffense, unitDefense, unitAttackRange);
        updateGraphicsView(sceneUnit.get(),ui->graphicsViewUnit);
    }
}// end of handleItemSelected

void MainWindow::textBrowserFieldUpdate (QString row,QString col,QString fieldTypeText, QString movementCost, QString fieldDefense)
{
    QString infoField = QString("line: %1, column: %2\nFieldType: %3\nMovementCost: %4\nFieldDefense: %5")
                           .arg(row)
                           .arg(col)
                           .arg(fieldTypeText)
                           .arg(movementCost)
                           .arg(fieldDefense);
    ui->textBrowserField->setText(infoField);
    ui->lcdNumber->display(playerBalances[countryOnTheTurn]);
}

void MainWindow::textBrowserUnitUpdate (QString unitText, QString unitStatus, QString unitMovement, QString unitExperience, QString unitOffense, QString unitDefense, QString unitAttackRange)
{
    if (!selectedUnit)
    {
        unitText="no unit";
        unitStatus="no unit";
        unitMovement="no unit";
        unitExperience="no unit";
        unitOffense="no unit";
        unitDefense="no unit";
        unitAttackRange="no unit";
    }
    QString infoUnit = QString("Unit: %1\nStatus: %2\nRange: %3\nExperience: %4\nOffense: %5\nDefense: %6\nAttack Range: %7")

                           .arg(unitText)
                           .arg(unitStatus)
                           .arg(unitMovement)
                           .arg(unitExperience)
                           .arg(unitOffense)
                           .arg(unitDefense)
                           .arg(unitAttackRange);
    ui->textBrowserUnit->setText(infoUnit);
}

void MainWindow::moveUnit(Unit *unit, int target_row, int target_col)
{
int distance = hexmap->calculateMovementCost(unit->getRow(),unit->getCol(),target_row,target_col,unit->getTerritory(),&Units);
unit->moveTo(target_row,target_col,distance);
hexmap->clearUnits();
hexmap->drawUnits(&Units);
ui->graphicsView->update();
}

void MainWindow::updateGraphicsView(QGraphicsScene *sceneUnit, QGraphicsView *view)
{
    if (selectedUnit)
    {
        itemUnit->setPixmap(UnitType::getPixmap(selectedUnit->getType()));
    }
    else
    {
        itemUnit->setPixmap(pixmapNoUnit);
    }
    //view->fitInView(itemUnit, Qt::KeepAspectRatio);
    view->update();
    sceneUnit->update();
    view->show();
}

void MainWindow::startCombat(Unit& attacker, Unit& defender)
{
    QPixmap *flagAttacker;
    QPixmap *flagDefender;
    if (countryOnTheTurn==country1)
    {
        flagAttacker=&pixmapCountry1;
        flagDefender=&pixmapCountry2;
    }
    else
    {
        flagAttacker=&pixmapCountry2;
        flagDefender=&pixmapCountry1;
    }
    CombatDialog combatDialog(attacker, defender, hexmap.get(),flagAttacker,flagDefender, this);
    if (combatDialog.exec() == QDialog::Accepted) {
        int damageDefender = combatDialog.getDamageDefener();
        int damageAttacker = combatDialog.getDamageAttacker();
        // Aktualisieren Sie den Zustand der verteidigenden Einheit basierend auf dem Schaden
        defender.setCurrentState(defender.getCurrentState() - damageDefender);
        attacker.setCurrentState(attacker.getCurrentState()- damageAttacker);
        attacker.increaseExperience();
        attacker.setActed();
    }


}

//manage a new turn
void MainWindow::onPushButtonNextTurnClicked()
{
    //InfoFenster Next Turn
    //QMessageBox::StandardButton reply;
    //reply = QMessageBox::question (this,"End turn!","Do you really want to end this turn",QMessageBox::Yes |QMessageBox::No);
    int reply;
    if (countryOnTheTurn==country1)
    {
        reply=CustomDialog::showDialogWithTwoButtons("Do you really want to end this turn?","Yes","No",":/Images/dialogbackground2",this);
    }
    else if (countryOnTheTurn==country2 && aiActivated)
    {
        reply=QDialog::Accepted;
    }
    else
    {
        reply=QDialog::Rejected;
    }

    if (reply==QDialog::Accepted)
    {
        
        
        //Bewegungspunkte der Einheiten auffrischen
        for (std::vector<Unit>::iterator it = Units.begin(); it!= Units.end(); ++it)//check if an unit was clicked
        {
            // Reset Movement Points
            it->setRemainingMovementPoints(UnitType::getRange(it->getType()));
            // Refresh Units state if rested
            if (!it->getActed() && it->getCountry()==countryOnTheTurn && it->getType()!=UnitType::militarybase)
            {
                it->setCurrentState(it->getCurrentState()+10);
                if (it->getCurrentState()>100) {it->setCurrentState(100);}
            }
            // delete acted flag
            it->deleteActed();
        }

        // calculate earnings and new balance
        playerBalances[countryOnTheTurn]+=10;
        calculateCityIncome();
        calculateIndustryIncome();

        //aktuelles Move Overlay löschen
        hexmap->clearActiveMoveOverlay();
        hexmap->clearActiveAttackOverlay();

        //change countryOnTheTurn
        if (countryOnTheTurn==country1)
        {
            countryOnTheTurn=country2;
            opponent=country1;
            itemFlag->setPixmap(pixmapCountry2);
        }
        else
        {
            countryOnTheTurn=country1;
            opponent=country2;
            itemFlag->setPixmap(pixmapCountry1);
            round ++;//Increase number of rounds
        }
        move=false;
        buyUnit=false;
        healing=false;
        selectedUnit=nullptr;
        textBrowserUnitUpdate("","","","","","","");
        updateGraphicsView(sceneUnit.get(),ui->graphicsViewUnit);
        sceneFlag->update();
        ui->graphicsViewFlag->update();
        ui->lcdNumber->display(playerBalances[countryOnTheTurn]);

        //AI Turn
        if (aiActivated && countryOnTheTurn==country2)
        {
            AIManager aiManager(this, hexmap.get(), &Units, countryOnTheTurn, opponent);

            // Process the AI's turn
            aiManager.processTurn();

            // Update the graphics view after AI actions
            ui->graphicsView->update();

            // Proceed to the next turn
            ui->pushButtonNextTurn->click();    
            
        }
    }
}// end of onPushButtonNextTurnClicked

void MainWindow::calculateCityIncome()
{
    for (auto& unit : Units) {
        if (unit.getCountry() == countryOnTheTurn) {
            Hex& hex = hexmap->getHex(unit.getRow(), unit.getCol());
            if (hex.getFieldType() == FieldType::City) {
                playerBalances[countryOnTheTurn] += 50; // Add 50 income for controlling a city
            }
        }
    }
}

void MainWindow::calculateIndustryIncome()
{
    for (auto& unit : Units) {
        if (unit.getCountry() == countryOnTheTurn) {
            Hex& hex = hexmap->getHex(unit.getRow(), unit.getCol());
            if (hex.getFieldType() == FieldType::Industry) {
                playerBalances[countryOnTheTurn] += 40; // Add 50 income for controlling a city
            }
        }
    }
}

//manage actions of the menu
void MainWindow::onActionTriggered()
{
    QAction *action = qobject_cast<QAction *>(sender());
        if (action)
        {
            if (action == exitAction.get())
            {
                int reply=CustomDialog::showDialogWithTwoButtons("Do you really want to exit the game?","Yes","No",":/Images/dialogbackground2",this);
                if (reply==QDialog::Accepted)
                {
                    close();
                }
            }
            else if (action == createNewMapAction.get())
            {
                int reply=CustomDialog::showDialogWithTwoButtons("Do you really want to create a new map?","Yes","No",":/Images/dialogbackground2",this);
                if (reply==QDialog::Accepted)
                {
                    //createNewMap();
                    startNewGame();
                }
            }
            else if (action == gameSaveAction.get())
            {
                saveAGame();
            }
            else if (action == gameLoadAction.get())
            {
                loadAGame();    
            }
        }


}

void MainWindow::loadAGame()
{
    if (gameMode)
    {
        stopGameMode();
    }
    
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open Game"), "", tr("Game Files (*.game)"));
    if (!fileName.isEmpty())
    {
        loadGame(fileName);
    }
    gameMode=true;
}

void MainWindow::saveAGame()
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save Game"), "", tr("Game Files (*.game)"));
    if (!fileName.isEmpty())
    {
         // Ensure the file has a .game extension
        if (!fileName.endsWith(".game", Qt::CaseInsensitive)) 
        {
            fileName += ".game";
        }
        saveGame(fileName);
    }
}

void MainWindow::onGearIconClicked()
{
    showStartScreen();
}

void MainWindow::startNewGame()
{
    createNewMap();    
    showStartScreen(); 
}

 //check if a unit died during combat
 void MainWindow::isAnybodyDead()
 {
     for (std::vector<Unit>::iterator it=Units.begin(); it!=Units.end();)
     {
         if (it->getCurrentState()<1)
         {
             //Headqaurter destroyed -> game ends
             if(it->getType()==UnitType::militarybase)
             {
                 QString winner;
                 if (it->getCountry()==country2)
                 {
                     winner=country1;
                 }
                 else
                 {
                     winner=country2;
                 }
                 winner += " wins";
                 //QMessageBox::information(this,"Game over!",winner);
                 CustomDialog::showDialogWithOneButton(winner,"OK",":/Images/dialogbackground2",this);
                 selectedUnit=nullptr;
                 
                gameMode=false;
                stopGameMode();
                showStartScreen();
                return;
                          
             }
             else
             {
             it=Units.erase(it);
             mediaPlayer->play();
             }
         }
         else
         {
             ++it;
         }
     }
     //update units on the map

     hexmap->clearUnits();
     hexmap->drawUnits(&Units);
 }

 void MainWindow::saveGame(const QString& fileName) {
     QFile file(fileName);
     if (!file.open(QIODevice::WriteOnly)) {
         QMessageBox::warning(this, tr("Save Game"), tr("Cannot open file for writing."));
         return;
     }

     QDataStream out(&file);
     out << *hexmap;
     out << static_cast<int>(Units.size());
     for (const Unit& unit : Units) {
         out << unit;
     }
     out << move << buyUnit << healing << aiActivated << countryOnTheTurn << opponent << round << playerBalances[country1] << playerBalances[country2];

     file.close();
 }

 void MainWindow::loadGame(const QString& fileName) {
     QFile file(fileName);
     if (!file.open(QIODevice::ReadOnly)) {
         QMessageBox::warning(this, tr("Load Game"), tr("Cannot open file for reading."));
         return;
     }

     QDataStream in(&file);
     in >> *hexmap;
     int unitCount;
     in >> unitCount;
     Units.resize(unitCount);
     for (size_t i = 0; i < static_cast<size_t>(unitCount); ++i)
     {
         in >> Units[i];
     }
     in >> move >> buyUnit >> healing >> aiActivated >> countryOnTheTurn >> opponent >> round >> playerBalances[country1] >> playerBalances[country2];

     file.close();
     // Aktualisiere die Darstellung nach dem Laden
     hexmap->hexItems.clear();
     drawMap();
     hexmap->drawUnits(&Units);
     hexmap->clearActiveMoveOverlay();
     hexmap->clearActiveAttackOverlay();
     ui->graphicsViewFlag->update();
     sceneFlag->update();
     updateGraphicsView(sceneUnit.get(),ui->graphicsViewUnit);
     textBrowserFieldUpdate("","","","","");
     textBrowserUnitUpdate("no unit","no unit","no unit","no unit","no unit","no unit","no unit");
     ui->graphicsView->show();

    }

void MainWindow::showStartScreen()
{
    if (editMapMode)
    {
        editMapMode = false;
        //QMessageBox::information(this, "Edit Map Mode", "Map Edit Mode deactivated.");
        CustomDialog::showDialogWithOneButton("Map Edit Mode deactivated.","OK",":Images/dialogbackground1.png",this);
    }
    mediaPlayer->setSource(QUrl("qrc:/sounds/blop.wav"));
    mediaPlayer->play();   
    StartScreen startScreen(this, this);
    startScreen.updateButtonStatus();
    if (startScreen.exec()!=QDialog::Accepted)
    {
        if (gameMode)
        {
            stopGameMode();
        }
        close();
        qDebug()<<"Game closed";
    }
}

void MainWindow::createNewMap()
{
    if (gameMode)
    {
        stopGameMode();
    }
    
    // Show the MapSizeDialog to get user input
    MapSizeDialog dialog(this);
    if (dialog.exec() != QDialog::Accepted) {
        return; // User canceled the dialog
    }

    // Get the width and height values from the dialog
    int width = dialog.getMapWidth();
    int height = dialog.getMapHeight();

   
    // Clear the old map
    hexmap->removeHexItemsFromScene(); // Remove all hex items from the scene
    hexmap->hexItems.clear();

    // Resize Hexmap with the given dimensions
    //qDebug() << "Creating a new map with dimensions: " << width << "x" << height << "in scene: " << scene.get();
    hexmap->resizeHexMap(width, height);
    
    
    //create and draw map
    hexmap->createRandomMap();
    drawMap();
    
    //create Units
    /*setStartUnits();
    hexmap->drawUnits(&Units);*/
    
    //set game variables and flags
    setGameVariables();
    itemFlag->setPixmap(pixmapCountry1);
    

    //update graphics
    hexmap->clearActiveMoveOverlay();
    hexmap->clearActiveAttackOverlay();
    ui->graphicsViewFlag->update();
    sceneFlag->update();
    updateGraphicsView(sceneUnit.get(),ui->graphicsViewUnit);
    textBrowserFieldUpdate("","","","","");
    textBrowserUnitUpdate("no unit","no unit","no unit","no unit","no unit","no unit","no unit");
    ui->graphicsView->show();
}

void MainWindow::startEditMapMode()
{
    editMapMode = true; // Enable edit mode
    //QMessageBox::information(this, "Edit Map Mode", "Map Edit Mode activated. Click on a field to change its terrain.");
    CustomDialog::showDialogWithOneButton("Map Edit Mode activated.<br>Click on a field to change its terrain.", "OK",":Images/dialogbackground1.png",this);
}

void MainWindow::editMap(HexItem* selectedItem)
{
    // Get the clicked field
        int row = selectedItem->getrow();
        int col = selectedItem->getcol();
        Hex& hex = hexmap->getHex(row, col);

        // Cycle through the available terrains
        int numberOfTypes = static_cast<int>(FieldType::Count);
        FieldType::Type currentType = hex.getFieldType();
        int nextTypeIndex = (static_cast<int>(currentType) + 1) % numberOfTypes;
        FieldType::Type nextType = static_cast<FieldType::Type>(nextTypeIndex);

        // Update the terrain type
        hex.setFieldType(nextType);

        // Update the graphics
        selectedItem->setPixmap(FieldType::getPixmap(nextType));
        ui->graphicsView->update();
}

void MainWindow::saveMap(const QString& fileName) 
{
     QFile file(fileName);
     if (!file.open(QIODevice::WriteOnly)) {
         QMessageBox::warning(this, tr("Save Map"), tr("Cannot open file for writing."));
         return;
     }

     QDataStream out(&file);
     out << *hexmap;
     
     file.close();
}

void MainWindow::loadMap(const QString& fileName) 
{
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly)) {
        QMessageBox::warning(this, tr("Load Map"), tr("Cannot open file for reading."));
        return;
    }

    QDataStream in(&file);
    in >> *hexmap;
    file.close();
    // Aktualisiere die Darstellung nach dem Laden
    hexmap->hexItems.clear();
    drawMap();
    //hexmap->drawUnits(&Units);
    hexmap->clearActiveMoveOverlay();
    hexmap->clearActiveAttackOverlay();
    ui->graphicsViewFlag->update();
    sceneFlag->update();
    updateGraphicsView(sceneUnit.get(),ui->graphicsViewUnit);
    textBrowserFieldUpdate("","","","","");
    textBrowserUnitUpdate("no unit","no unit","no unit","no unit","no unit","no unit","no unit");
    ui->graphicsView->show();

}

void MainWindow::loadAMap()
{
    if (gameMode)
    {
        stopGameMode();
    }
    
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open Map"), "", tr("Map Files (*.map)"));
    if (!fileName.isEmpty())
    {
        loadMap(fileName);
    }
}

void MainWindow::saveAMap()
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save Map"), "", tr("Map Files (*.map)"));
    if (!fileName.isEmpty())
    {
         // Ensure the file has a .game extension
        if (!fileName.endsWith(".map", Qt::CaseInsensitive)) 
        {
            fileName += ".map";
        }
        saveGame(fileName);
    }
}

void MainWindow::startGameMode()
{
    if (editMapMode)
    {
    editMapMode = false; // Disable edit mode
    }
    if (gameMode)
    {
        stopGameMode();
    }
    gameMode = true; // Enable game mode
    setStartUnits();
    hexmap->drawUnits(&Units);
}

void MainWindow::stopGameMode()
{
    
    if (gameMode)
    {
        //QMessageBox::StandardButton reply;
        //reply = QMessageBox::question (this,"Stop Game!","Do you want to save the game?",QMessageBox::Yes |QMessageBox::No);
        int reply=CustomDialog::showDialogWithTwoButtons("Do you want to save the game?","Yes","No",":/Images/dialogbackground1",this);
        if (reply==QDialog::Accepted)
        {
            saveAGame();
        }
    }
    gameMode = false; // Disable game mode
    Units.clear();
    hexmap->clearUnits();
    hexmap->clearActiveOverlay();
    hexmap->clearActiveMoveOverlay();
    hexmap->clearActiveAttackOverlay();
}

bool MainWindow::getGameModeStatus()
{
    return gameMode;
}

void MainWindow::setGameVariables()
{
    round=1;
    playerBalances["Lupony"]=400;
    playerBalances["Ursony"]=100;
    countryOnTheTurn=country1;
    opponent=country2;
    move=false;
    buyUnit=false;
    healing=false;
    selectedUnit=nullptr;
}

int MainWindow::getPlayerBalances(QString country)
{
    return playerBalances[country];
}

void MainWindow::setPlayerBalances(QString country, int balance)
{
    playerBalances[country]=balance;
}