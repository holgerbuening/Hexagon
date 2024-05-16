#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "zoomablegraphicsview.h"
#include "fieldtype.h"
#include "hexmap.h"
#include "hexitem.h"
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include "unittype.h"
#include "unit.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    scene(new QGraphicsScene(this)),
    hexmap(new HexMap(10,6,scene)),
    infantry("Infanterie", 50,25,5,1,QPixmap(":/images/infantry.png"))
{

    ui->setupUi(this);
    FieldType::loadPixmaps();
    connect(ui->radioButton, &QRadioButton::toggled, this, &MainWindow::onRadioButtonToggled);

    //create and draw map
    hexmap->createRandomMap();
    drawMap();

    //create Units
    Unit infantryUnit(infantry, 2, 2);
    Unit inf2(infantry,4,4);
    Units.push_back(infantryUnit);
    Units.push_back(inf2);
    drawUnits();

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

void MainWindow::drawUnits()
{
    for (std::vector<Unit>::iterator it = Units.begin(); it!= Units.end(); ++it)
    {
        int col=it->getCol();
        int row=it->getRow();
        int x = col * hexmap->getXOffset();
        int y = row * hexmap->getYOffset() + (col % 2) * (hexmap->getHexHeight() / 2);
        QGraphicsPixmapItem* item = scene->addPixmap(it->getType().getPixmap());
        item->setPos(x, y);
        //scene->addItem(item);
    }
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
    hexmap->setActiveOverlay(selectedItem->overlayItem);
    int row= selectedItem->getrow();
    int col= selectedItem->getcol();
    QString fieldTypeText=hexmap->getHex(row,col).getFieldTypeText();
    int movementCost = hexmap->getHex(row,col).getMovementCost();
    QString info = QString("Zeile: %1, Spalte: %2\nFieldType: %3\nMovementCost: %4")
                           .arg(row)
                           .arg(col)
                           .arg(fieldTypeText)  // Du musst möglicherweise eine Methode implementieren, um FieldType zu konvertieren
                           .arg(movementCost);
    //ui->textBrowser->clear();
    ui->textBrowser->setText(info);
}

