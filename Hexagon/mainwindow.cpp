#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "zoomablegraphicsview.h"
#include "fieldtype.h"
#include "hexmap.h"
#include "hexitem.h"
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QMessageBox>
#include "unittype.h"
#include "unit.h"
#include "combatdialog.h"
#include <random>
#include <iostream>
#include <QThread>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    scene(new QGraphicsScene(this)),
    sceneUnit(new QGraphicsScene(this)),
    sceneFlag(new QGraphicsScene(this)),
    hexmap(new HexMap(20,12,scene))
{
    // initial settings
    ui->setupUi(this);
    FieldType::loadPixmaps();
    UnitType::loadUnits();
    aiActivated=true;


    // create menu
    menuBar = new QMenuBar(this);
    setMenuBar(menuBar);
    gameMenu = new QMenu("Game", this);
    exitAction = new QAction("Exit", this);
    gameMenu->addAction(exitAction);
    menuBar->addMenu(gameMenu);
    mapMenu = new QMenu("Map",this);
    createNewMapAction = new QAction("Create new map",this);
    mapMenu->addAction(createNewMapAction);
    menuBar->addMenu(mapMenu);


     //Signal - Slot Connections
    connect(ui->radioButton, &QRadioButton::toggled, this, &MainWindow::onRadioButtonToggled);
    connect(ui->pushButtonNextTurn, &QPushButton::clicked, this, &MainWindow::onPushButtonNextTurnClicked);
    connect(exitAction, &QAction::triggered, this, &MainWindow::onActionTriggered);
    connect(createNewMapAction, &QAction::triggered, this, &MainWindow::onActionTriggered);

    //create and draw map
    hexmap->createRandomMap();
    drawMap();

    //set game variables and flags
    country1="Lupony";
    country2="Ursony";
    round=1;
    countryOnTheTurn=country1;
    opponent=country2;
    pixmapCountry1= QPixmap(":/Images/flag_lupony.png");
    pixmapCountry2= QPixmap(":/Images/flag_ursony.png");
    itemFlag = new QGraphicsPixmapItem(pixmapCountry1);
    move=false;
    selectedUnit=nullptr;

    //create Units
    setStartUnits();
    hexmap->drawUnits(&Units);

    //prepare main View
    ui->graphicsView->setScene(scene);
    ui->graphicsView->setRenderHint(QPainter::Antialiasing);
    ui->graphicsView->setBackgroundBrush(Qt::lightGray);
    ui->graphicsView->show();

    //prepare sidebar Views
    pixmapNoUnit =  QPixmap(":/Images/noUnit.png");
    itemUnit = new QGraphicsPixmapItem(pixmapNoUnit);
    ui->graphicsViewUnit->setScene(sceneUnit);
    sceneUnit->addItem(itemUnit);
    ui->graphicsViewUnit->fitInView(itemUnit, Qt::KeepAspectRatio);
    ui->graphicsViewUnit->setRenderHint(QPainter::Antialiasing);
    ui->graphicsViewUnit->show();
    ui->graphicsViewFlag->setScene(sceneFlag);
    sceneFlag->addItem(itemFlag);
    ui->graphicsViewFlag->show();

}

void MainWindow::setStartUnits()
{
bool found=false;
Units.clear();
int rowBase1;
int colBase1;

    //find base for country1
    while (!found)
    {
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
                Unit Base(UnitType::militarybase,row,col,country1);
                Units.push_back(inf1);
                Units.push_back(inf2);
                Units.push_back(Base);
                rowBase1=row;
                colBase1=col;
                found=true;
            }

        }
    }
    //find base for country2
    found=false;
    while (!found)
    {

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
                    Unit Base(UnitType::militarybase,row,col,country2);
                    Units.push_back(inf1);
                    Units.push_back(inf2);
                    Units.push_back(Base);
                    found=true;
                }
            }
        }
    }
}

MainWindow::~MainWindow()
{
    delete ui;
    delete hexmap;
    hexmap = nullptr;
}

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
                HexItem* item = new HexItem(FieldType::getPixmap(hex.getFieldType()), nullptr, row,col);
                item->setPos(x, y);
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

    //clicked on a unit but no unit selected so far -> first step of movement/attack
    if (!move && unit_clicked && selectedUnitThisClick->getCountry()==countryOnTheTurn && !selectedUnitThisClick->getActed())//no unit selected so far -> first stpe of movement
    {
        move=true;
        hexmap->setActiveOverlay(selectedItem->overlayItem);
        hexmap->drawActiveMoveOverlay(row,col,distance,territory, &Units);
        hexmap->drawActiveAttackOverlay(row,col,selectedUnitThisClick->getAttackRange(),opponent,&Units);
        selectedUnit=selectedUnitThisClick; //mark the selected Unit for the move process
        selectedUnitCol=col; //mark the actual position of the selected Unit
        selectedUnitRow=row;
    }

    //clicked on a unit unit acted already -> no action only information
    else if (!move && unit_clicked && selectedUnitThisClick->getCountry()==countryOnTheTurn && selectedUnitThisClick->getActed())//no unit selected so far -> first stpe of movement
    {
        hexmap->setActiveOverlay(selectedItem->overlayItem);
        selectedUnit=selectedUnitThisClick; //mark the selected Unit for the move process
        selectedUnitCol=col; //mark the actual position of the selected Unit
        selectedUnitRow=row;
    }


    //second selection om the same unit -> Deselection
    else if (move && unit_clicked && selectedUnitCol==col && selectedUnitRow==row && selectedUnitThisClick->getCountry()==countryOnTheTurn)//second selection om the same unit -> Deselection
    {
        hexmap->clearActiveMoveOverlay();
        hexmap->clearActiveAttackOverlay();
        hexmap->setActiveOverlay(selectedItem->overlayItem);
        move=false;
        selectedUnit=nullptr;
    }

    //different unit selected during move process -> no Movement -> new Unit is selected for Move process
    else if (move && unit_clicked && selectedUnitThisClick->getCountry()==countryOnTheTurn && !selectedUnitThisClick->getActed())
    {
        hexmap->clearActiveMoveOverlay();
        hexmap->clearActiveAttackOverlay();
        selectedUnit=selectedUnitThisClick;
        selectedUnitRow=row;
        selectedUnitCol=col;
        move=true;
        hexmap->setActiveOverlay(selectedItem->overlayItem);
        hexmap->drawActiveMoveOverlay(row,col,distance,territory, &Units);
        hexmap->drawActiveAttackOverlay(row,col,selectedUnitThisClick->getAttackRange(),opponent,&Units);
    }

    //different unit selected during move process -> no Movement -> new Unit has no action points left -> information only
    else if (move && unit_clicked && selectedUnitThisClick->getCountry()==countryOnTheTurn && selectedUnitThisClick->getActed())
    {
        hexmap->clearActiveMoveOverlay();
        hexmap->clearActiveAttackOverlay();
        selectedUnit=selectedUnitThisClick;
        selectedUnitRow=row;
        selectedUnitCol=col;
        move=false;
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
        if (move==false)
        {
           hexmap->setActiveOverlay(selectedItem->overlayItem);
           selectedUnit=nullptr;
        }

        // clicked onto an empty field and a unit was selected before (possible move process)
        else
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
    }// end of clicked on empty field

    //something happended we did not foresee
    else
    {
        hexmap->clearActiveMoveOverlay();
        hexmap->clearActiveAttackOverlay();
        hexmap->setActiveOverlay(selectedItem->overlayItem);
        selectedUnit=nullptr;
        move=false;
        unitText="Unforeseen Event";
    }

    textBrowserFieldUpdate(QString::number(row),QString::number(col),fieldTypeText,QString::number(movementCost), QString::number(fieldDefense));
    textBrowserUnitUpdate(unitText,unitStatus,unitMovement, unitExperience, unitOffense, unitDefense, unitAttackRange);
    updateGraphicsView(sceneUnit,ui->graphicsViewUnit);

}

void MainWindow::textBrowserFieldUpdate (QString row,QString col,QString fieldTypeText, QString movementCost, QString fieldDefense)
{
    QString infoField = QString("line: %1, column: %2\nFieldType: %3\nMovementCost: %4\nFieldDefense: %5")
                           .arg(row)
                           .arg(col)
                           .arg(fieldTypeText)
                           .arg(movementCost)
                           .arg(fieldDefense);
    ui->textBrowserField->setText(infoField);
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
    CombatDialog combatDialog(attacker, defender, hexmap,flagAttacker,flagDefender, this);
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
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question (this,"End turn!","Do you really want to end this turn",QMessageBox::Yes |QMessageBox::No);

    if (reply==QMessageBox::Yes)
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
        selectedUnit=nullptr;
        textBrowserUnitUpdate("","","","","","","");
        updateGraphicsView(sceneUnit,ui->graphicsViewUnit);
        sceneFlag->update();
        ui->graphicsViewFlag->update();

        //AI Turn
        if (aiActivated && countryOnTheTurn==country2)
        {

            std::vector<Unit*> enemyUnits;
            std::vector<Unit*> ownUnits;
            std::vector<Hex> objectives;

            // Fülle die Vektoren mit Zeigern auf die Einheiten und Ziele
            for (std::vector<Unit>::iterator it = Units.begin(); it != Units.end(); ++it)
            {
                if (it->getType() == UnitType::militarybase && it->getCountry() == country1)
                {
                    objectives.push_back(hexmap->getHex(it->getRow(), it->getCol()));
                }
                else if (it->getCountry() == country1)
                {
                    enemyUnits.push_back(&(*it));  // Zeiger auf das Element hinzufügen
                }
                else if (it->getType() != UnitType::militarybase)
                {
                    ownUnits.push_back(&(*it));  // Zeiger auf das Element hinzufügen
                }
            }
            //Ai for each own unit
            if (!ownUnits.empty())
            {
                for (std::vector<Unit*>::iterator it=ownUnits.begin();it!=ownUnits.end();++it)
                {
                    AIState state=aiDetermineState(*it,enemyUnits,objectives);
                    aiPerformAction((*it),state,enemyUnits,objectives);
                }
                //QThread::sleep(5);
                ui->graphicsView->update();
                ui->pushButtonNextTurn->click();
            }
        }

    }
}

void MainWindow::onActionTriggered()
{
    QAction *action = qobject_cast<QAction *>(sender());
        if (action)
        {
            if (action == exitAction)
            {
                QMessageBox::StandardButton reply;
                reply = QMessageBox::question (this,"Exit Game!","Do you really want to exit the game?",QMessageBox::Yes |QMessageBox::No);

                if (reply==QMessageBox::Yes)
                {
                    close();
                }
            }
            else if (action == createNewMapAction)
            {
                QMessageBox::StandardButton reply;
                reply = QMessageBox::question (this,"Create a new map!","Do you really want to create a new map?",QMessageBox::Yes |QMessageBox::No);

                if (reply==QMessageBox::Yes)
                {
                   startNewGame();
                }
            }
        }


}

void MainWindow::startNewGame()
 {
     hexmap->hexItems.clear();
     hexmap->createRandomMap();
     drawMap();
     setStartUnits();
     hexmap->drawUnits(&Units);
     round=1;
     countryOnTheTurn=country1;
     itemFlag->setPixmap(pixmapCountry1);
     opponent=country2;
     move=false;
     selectedUnit=nullptr;
     hexmap->clearActiveMoveOverlay();
     hexmap->clearActiveAttackOverlay();
     ui->graphicsViewFlag->update();
     sceneFlag->update();
     updateGraphicsView(sceneUnit,ui->graphicsViewUnit);
     textBrowserFieldUpdate("","","","","");
     textBrowserUnitUpdate("no unit","no unit","no unit","no unit","no unit","no unit","no unit");
     ui->graphicsView->show();
 }

 AIState MainWindow::aiDetermineState(Unit* unit, std::vector<Unit*>enemyUnits, std::vector<Hex> objectives)
 {
     // Beispielhafte Zustandsbestimmung
     if (unit->getCurrentState() < 30)
     {
         return RETREAT;
     }
     else if (!enemyUnits.empty())
     {
         return ATTACK;
     }
     else if (!objectives.empty())
     {
         return CAPTURE;
     }
     else
     {
     return DEFEND;
     }
 }

 void MainWindow::aiPerformAction(Unit* unit, AIState state, std::vector<Unit*> enemyUnits, std::vector<Hex> objectives) {
     switch (state) {
         case ATTACK:
             // find opponent
             if (!enemyUnits.empty())
             {
                 Hex start = hexmap->getHex(unit->getRow(),unit->getCol());
                 int territory = start.getTerritory();
                 Unit* firstEnemyUnitPtr = *enemyUnits.begin();
                 Unit objective = *firstEnemyUnitPtr; // Dereferenziere den Zeiger, um auf das Unit-Objekt zuzugreifen
                 Hex objectiveHex = hexmap->getHex(objective.getRow(),objective.getCol());
                 Hex target = hexmap->getClosestNeighbourSameTerritoryNoUnits(start,objectiveHex,territory,&Units);
                 int attackRow = target.getRow();
                 int attackCol = target.getCol();
                 std::vector<Node> wayToTarget = hexmap->AStar(start,hexmap->getHex(attackRow,attackCol),unit->getTerritory(), &Units);
                 if (!wayToTarget.empty())
                 {
                     Node targetNextMove = hexmap->getReachableNode(wayToTarget,unit->getRemainingMovementPoints());
                     //int distance=hexmap->calculateMovementCost(unit.getRow(),unit.getCol(),targetNextMove.row,targetNextMove.col,territory,&Units);
                     moveUnit(unit,targetNextMove.row,targetNextMove.col);
                     if (unit->getRemainingMovementPoints()<=0)
                     {
                         unit->setActed();
                     }
                     else if (hexmap->calculateMovementCost(targetNextMove.row,targetNextMove.col,objectiveHex.getRow(),objectiveHex.getCol(),territory,&Units)<=unit->getAttackRange()
                              && hexmap->calculateMovementCost(targetNextMove.row,targetNextMove.col,objectiveHex.getRow(),objectiveHex.getCol(),territory,&Units)!=-1)
                     {
                         startCombat(*unit,objective);
                         isAnybodyDead();
                     }
                 }

             }
             break;
         case DEFEND:
             // Verteidige Position oder bewege dich zu einer defensiven Position
             break;
         case CAPTURE:
             // Bewege die Einheit zum nächstgelegenen Ziel
             if (!objectives.empty()) {
                 const Hex& objective = *objectives.begin(); // Beispiel: Wähle das erste Ziel
                 int captureRow = objective.getRow();
                 int captureCol = objective.getCol();
                 // Bewegung zum Ziel
                 // Hier A*-Algorithmus zur Bewegung verwenden
             }
             break;
         case RETREAT:
             // Bewege die Einheit zur nächsten sicheren Position oder Basis
             break;
     }
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
                 QMessageBox::information(this,"Game over!",winner);
                 selectedUnit=nullptr;

                 startNewGame();
             }
             else
             {
             it=Units.erase(it);
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
