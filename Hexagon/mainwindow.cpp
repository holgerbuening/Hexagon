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
    hexmap(new HexMap(10,6,scene))
{
    move=false;
    selectedUnit=nullptr;
    //itemNoUnit=new QGraphicsTextItem("no unit selected!");
    //itemNoUnit->setDefaultTextColor(Qt::red); // Setzen der Textfarbe
    //itemNoUnit->setFont(QFont("Arial", 20)); // Setzen der Schriftart und Größe
    ui->setupUi(this);
    FieldType::loadPixmaps();
    UnitType::loadUnits();
    pixmapNoUnit =  QPixmap(":/Images/noUnit.png");
    itemUnit = new QGraphicsPixmapItem(pixmapNoUnit);


    //Signal - Slot Connections
    connect(ui->radioButton, &QRadioButton::toggled, this, &MainWindow::onRadioButtonToggled);
    connect(ui->pushButtonNextTurn, &QPushButton::clicked, this, &MainWindow::onPushButtonNextTurnClicked);
    //create and draw map
    hexmap->createRandomMap();
    drawMap();

    //create Units
    Unit infantryUnit(UnitType::infantry, 2, 2);
    Unit inf2(UnitType::infantry,4,4);
    Units.push_back(infantryUnit);
    Units.push_back(inf2);
    hexmap->drawUnits(&Units);

    ui->graphicsView->setScene(scene);
    ui->graphicsView->setRenderHint(QPainter::Antialiasing);
    ui->graphicsView->setBackgroundBrush(Qt::lightGray);
    ui->graphicsView->show();

    ui->graphicsViewUnit->setScene(sceneUnit);
    sceneUnit->addItem(itemUnit);
    ui->graphicsViewUnit->fitInView(itemUnit, Qt::KeepAspectRatio);
    ui->graphicsViewUnit->setRenderHint(QPainter::Antialiasing);
    ui->graphicsViewUnit->show();


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

void MainWindow::handleItemSelected(HexItem* selectedItem)
{
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

    for (std::vector<Unit>::iterator it = Units.begin(); it!= Units.end(); ++it)//check if an unit was clicked
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


    if (not move && unit_clicked )//no unit selected so far -> first stpe of movement
    {
        move=true;
        hexmap->setActiveOverlay(selectedItem->overlayItem);
        hexmap->drawActiveMoveOverlay(row,col,distance,territory);
        selectedUnit=selectedUnitThisClick; //mark the selected Unit for the move process
        selectedUnitCol=col; //mark the actual position of the selected Unit
        selectedUnitRow=row;
     }
     else if (move && unit_clicked && selectedUnitCol==col && selectedUnitRow==row)//second selection om the same unit -> Deselection
     {
        hexmap->clearActiveMoveOverlay();
        hexmap->setActiveOverlay(selectedItem->overlayItem);
        move=false;
        selectedUnit=nullptr;
     }
     else if (move && unit_clicked) //different unit selected during move process -> no Movement -> new Unit is selected for Move process
     {
        hexmap->clearActiveMoveOverlay();
        selectedUnit=selectedUnitThisClick;
        selectedUnitRow=row;
        selectedUnitCol=col;
        move=true;
        hexmap->setActiveOverlay(selectedItem->overlayItem);
        hexmap->drawActiveMoveOverlay(row,col,distance,territory);
      }
      else if (not unit_clicked) //clicked not onto a unit
      {
        if (move==false) //no unit selected so far -> not in move process
        {
           hexmap->setActiveOverlay(selectedItem->overlayItem);
           selectedUnit=nullptr;
        }
        else // a unit was selected before
        {
            if (hexmap->calculateMovementCost(selectedUnitRow,selectedUnitCol,row,col,selectedUnit->getTerritory())<=selectedUnit->getRemainingMovementPoints()
                    && selectedUnit->getTerritory()==FieldType::getTerritory(hexmap->getHex(row,col).getFieldType())) //move Unit
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
            else  //field out of range clicked
            {
            hexmap->clearActiveMoveOverlay();
            hexmap->setActiveOverlay(selectedItem->overlayItem);
            selectedUnit=nullptr;
            move=false;
            }
        }
      }


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
