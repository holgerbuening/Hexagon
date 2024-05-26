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

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    scene(new QGraphicsScene(this)),
    sceneUnit(new QGraphicsScene(this)),
    sceneFlag(new QGraphicsScene(this)),
    hexmap(new HexMap(10,6,scene))
{
    // initial settings
    ui->setupUi(this);
    FieldType::loadPixmaps();
    UnitType::loadUnits();

    //Signal - Slot Connections
    connect(ui->radioButton, &QRadioButton::toggled, this, &MainWindow::onRadioButtonToggled);
    connect(ui->pushButtonNextTurn, &QPushButton::clicked, this, &MainWindow::onPushButtonNextTurnClicked);

    //create and draw map
    hexmap->createRandomMap();
    drawMap();

    //set game variables and flags
    country1="Lupony";
    country2="Ursony";
    round=1;
    countryOnTheTurn=country1;
    pixmapCountry1= QPixmap(":/Images/flag_lupony.png");
    pixmapCountry2= QPixmap(":/Images/flag_ursony.png");
    itemFlag = new QGraphicsPixmapItem(pixmapCountry1);
    move=false;
    selectedUnit=nullptr;

    //create Units
    Unit infantryUnit(UnitType::infantry, 2, 2, country1);
    Unit inf2(UnitType::infantry,5,7,country2);
    Units.push_back(infantryUnit);
    Units.push_back(inf2);
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
    int row= selectedItem->getrow();
    int col= selectedItem->getcol();
    int territory;
    const Hex& hex = hexmap->getHex(row, col);
    QString fieldTypeText=hex.getFieldTypeText();
    int movementCost = hex.getMovementCost();

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
            distance=it->getRemainingMovementPoints();
            unitMovement=QString::number(distance);
        }
    }

    //clicked on a unit but no unit selected so far -> first step of movement
    if (not move && unit_clicked && selectedUnitThisClick->getCountry()==countryOnTheTurn)//no unit selected so far -> first stpe of movement
    {
        move=true;
        hexmap->setActiveOverlay(selectedItem->overlayItem);
        hexmap->drawActiveMoveOverlay(row,col,distance,territory);
        selectedUnit=selectedUnitThisClick; //mark the selected Unit for the move process
        selectedUnitCol=col; //mark the actual position of the selected Unit
        selectedUnitRow=row;
    }

    //second selection om the same unit -> Deselection
    else if (move && unit_clicked && selectedUnitCol==col && selectedUnitRow==row && selectedUnitThisClick->getCountry()==countryOnTheTurn)//second selection om the same unit -> Deselection
    {
        hexmap->clearActiveMoveOverlay();
        hexmap->setActiveOverlay(selectedItem->overlayItem);
        move=false;
        selectedUnit=nullptr;
    }

    //different unit selected during move process -> no Movement -> new Unit is selected for Move process
    else if (move && unit_clicked && selectedUnitThisClick->getCountry()==countryOnTheTurn)
    {
        hexmap->clearActiveMoveOverlay();
        selectedUnit=selectedUnitThisClick;
        selectedUnitRow=row;
        selectedUnitCol=col;
        move=true;
        hexmap->setActiveOverlay(selectedItem->overlayItem);
        hexmap->drawActiveMoveOverlay(row,col,distance,territory);
    }

    //opponent unit selected during move process -> ATTACK^
    else if (move && unit_clicked && selectedUnitThisClick->getCountry()!=countryOnTheTurn)
    {
        hexmap->setActiveOverlay(selectedItem->overlayItem);
        hexmap->clearActiveOverlay();
        unitText="no unit";
        unitStatus="no unit";
        unitMovement="no unit";
        selectedUnit=nullptr;
        hexmap->clearActiveMoveOverlay();
        move=false;
    }

    //opponent unit selected while NOT in move process -> clear overlay
    else if (!move && unit_clicked && selectedUnitThisClick->getCountry()!=countryOnTheTurn)
    {
        hexmap->setActiveOverlay(selectedItem->overlayItem);
        hexmap->clearActiveOverlay();
        unitText="no unit";
        unitStatus="no unit";
        unitMovement="no unit";
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

            //clicked onto an empty field and a unit was selected before, the field is within this units range ->move Unit
            if (hexmap->calculateMovementCost(selectedUnitRow,selectedUnitCol,row,col,selectedUnit->getTerritory())<=selectedUnit->getRemainingMovementPoints()
                    && selectedUnit->getTerritory()==FieldType::getTerritory(hexmap->getHex(row,col).getFieldType()))
            {
                moveUnit(selectedUnit,row,col);
                unitText=selectedUnit->getUnitTypeText();
                unitStatus=QString::number(selectedUnit->getCurrentState());
                unitMovement=QString::number(selectedUnit->getRemainingMovementPoints());
                //selectedUnit=nullptr;
                move=false;
                hexmap->clearActiveMoveOverlay();
                hexmap->setActiveOverlay(selectedItem->overlayItem);

            }

            //clicked onto an empty field and a unit was selected before, the field is NOT in this units range ->no move but plain field information
            else
            {
            hexmap->clearActiveMoveOverlay();
            hexmap->setActiveOverlay(selectedItem->overlayItem);
            selectedUnit=nullptr;
            move=false;
            }
        }//end of clicked on empty field while in move process
    }// end of clicked on empty field


    textBrowserFieldUpdate(QString::number(row),QString::number(col),fieldTypeText,QString::number(movementCost));
    textBrowserUnitUpdate(unitText,unitStatus,unitMovement);
    updateGraphicsView(sceneUnit,ui->graphicsViewUnit);

}

void MainWindow::textBrowserFieldUpdate (QString row,QString col,QString fieldTypeText, QString movementCost)
{
    QString infoField = QString("line: %1, column: %2\nFieldType: %3\nMovementCost: %4")
                           .arg(row)
                           .arg(col)
                           .arg(fieldTypeText)  // Du musst möglicherweise eine Methode implementieren, um FieldType zu konvertieren
                           .arg(movementCost);
    ui->textBrowserField->setText(infoField);
}

void MainWindow::textBrowserUnitUpdate (QString unitText, QString unitStatus, QString unitMovement)
{
    QString infoUnit = QString("Unit: %1\nStatus: %2\nRange: %3")

                           .arg(unitText)
                           .arg(unitStatus)
                           .arg(unitMovement);
    ui->textBrowserUnit->setText(infoUnit);
}


void MainWindow::moveUnit(Unit *unit, int target_row, int target_col)
{
int distance = hexmap->calculateMovementCost(unit->getRow(),unit->getCol(),target_row,target_col,unit->getTerritory());
unit->moveTo(target_row,target_col,distance);
hexmap->clearUnits();
hexmap->drawUnits(&Units);
}

void MainWindow::onPushButtonNextTurnClicked()
{
    //InfoFenster Next Turn
    QMessageBox::information(this, "Info", "Next Turn", QMessageBox::Ok);

    //Bewegungspunkte der Einheiten auffrischen
    for (std::vector<Unit>::iterator it = Units.begin(); it!= Units.end(); ++it)//check if an unit was clicked
    {
        it->setRemainingMovementPoints(UnitType::getRange(it->getType()));
    }

    //aktuelles Move Overlay löschen
    hexmap->clearActiveMoveOverlay();

    //change countryOnTheTurn
    if (countryOnTheTurn==country1)
    {
        countryOnTheTurn=country2;
        itemFlag->setPixmap(pixmapCountry2);
    }
    else
    {
        countryOnTheTurn=country1;
        itemFlag->setPixmap(pixmapCountry1);
        round ++;//Increase number of rounds
    }
    ui->graphicsViewFlag->update();
    sceneFlag->update();
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
}
