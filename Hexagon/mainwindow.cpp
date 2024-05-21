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
    hexmap(new HexMap(10,6,scene))
{
    move=false;
    selectedUnit=nullptr;
    ui->setupUi(this);
    FieldType::loadPixmaps();
    UnitType::loadUnits();

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
    ui->graphicsView->setBackgroundBrush(QColor(230, 200, 167));
    ui->graphicsView->show();


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
                Hex& hex = hexmap->getHex(row, col);
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
    QString unitText("no Unit");
    QString unitStatus("no unit");
    QString unitMovement("no unit");
    int row= selectedItem->getrow();
    int col= selectedItem->getcol();
    int territory;
    QString fieldTypeText=hexmap->getHex(row,col).getFieldTypeText();
    int movementCost = hexmap->getHex(row,col).getMovementCost();

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


    if (not move && unit_clicked )//no unit selected so far
    {
        move=true;
        hexmap->setActiveOverlay(selectedItem->overlayItem);
        hexmap->drawActiveMoveOverlay(row,col,distance,territory);
        selectedUnit=selectedUnitThisClick;
        selectedUnitCol=col;
        selectedUnitRow=row;
     }
     else if (move && unit_clicked && selectedUnitCol==col && selectedUnitRow==row)//second selection om the same unit -> Deselection
     {
        hexmap->clearActiveMoveOverlay();
        hexmap->setActiveOverlay(selectedItem->overlayItem);
        move=false;
        selectedUnit=nullptr;
     }
     else if (move && unit_clicked) //different unit selected
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
        if (move==false) //no unit selected so far
        {
           hexmap->setActiveOverlay(selectedItem->overlayItem);
           selectedUnit=nullptr;
        }
        else // a unit was selected before
        {
            if (hexmap->distance(row,col,selectedUnitRow,selectedUnitCol)<=selectedUnit->getRemainingMovementPoints()
                    && selectedUnit->getTerritory()==FieldType::getTerritory(hexmap->getHex(row,col).getFieldType())) //move Unit
            {
                moveUnit(selectedUnit,row,col);
                selectedUnit=nullptr;
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


    QString info = QString("Zeile: %1, Spalte: %2\nFieldType: %3\nMovementCost: %4\nUnit: %5\nStatus: %6\nMovePoints: %7")
                           .arg(row)
                           .arg(col)
                           .arg(fieldTypeText)  // Du musst möglicherweise eine Methode implementieren, um FieldType zu konvertieren
                           .arg(movementCost)
                           .arg(unitText)
                           .arg(unitStatus)
                           .arg(unitMovement)
                           ;


    ui->textBrowser->setText(info);

}

void MainWindow::moveUnit(Unit *unit, int target_row, int target_col)
{
int distance = hexmap->distance(unit->getRow(),unit->getCol(),target_row,target_col);
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
}
