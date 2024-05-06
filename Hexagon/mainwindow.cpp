#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "zoomablegraphicsview.h"
#include "fieldtype.h"
#include "hexmap.h"
#include "hexitem.h"
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    scene(new QGraphicsScene(this)),
    hexmap(new HexMap(10,6,scene))
{

    ui->setupUi(this);
    FieldType::loadPixmaps();
    connect(ui->radioButton, &QRadioButton::toggled, this, &MainWindow::onRadioButtonToggled);

    hexmap->createRandomMap();
    drawMap();
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
