#include "hexmap.h"
#include <QGraphicsPixmapItem>
#include <QGraphicsScene>
#include "hexitem.h"
#include <cstdlib> // Für rand() und srand()
#include <ctime> // Für time()

HexMap::HexMap(int width, int height, QGraphicsScene* scene_v)
 : width(width), height(height),  gridPixmap(":/hexfields/Images/grid_big.png"), movePixmap(":/hexfields/Images/grid_big_move.png")
 {
    activeOverlayItem=nullptr;
    scene=scene_v;
    map.resize(height);
    for (int y = 0; y < height; ++y) {
        map[y].resize(width);
        for (int x = 0; x < width; ++x) {
            map[y][x] = Hex(x, y, FieldType::Woods, 1); // Beispielparameter anpassen
        }
    }
}




void HexMap::createRandomMap()
{
    srand(time(NULL));
    for (int y=0; y< height; y++)
    {
        for (int x=0; x< width; x++)
        {
            int randomType = rand() % 4;
            map[y][x].setFieldType(static_cast<FieldType::Type>(randomType));
        }
    }
}

void HexMap::drawMap()
{
    if (hexItems.empty())  // Erstelle die HexItems, wenn sie noch nicht existieren
    {
        for (int row = 0; row < height; ++row)
        {
            for (int col = 0; col < width; ++col)
            {
                Hex& hex = map[row][col];
                int x = col * xOffset;
                int y = row * yOffset + (col % 2) * (hexHeight / 2);
                HexItem* item = new HexItem(FieldType::getPixmap(hex.getFieldType()), nullptr);
                item->setPos(x, y);
                hexItems.push_back(item);
            }
        }
    }
    addHexItemsToScene();  // Fügt bestehende HexItems zur Szene hinzu
}

void HexMap::drawGrid()
{
    if(gridItems.empty())
    {
        for (int row = 0; row < height; ++row)
        {
            for (int col = 0; col < width; ++col)
            {
                int x = col * xOffset;
                int y = row * yOffset + (col % 2) * (hexHeight / 2); // Versetzung für ungerade Spalten
                QGraphicsPixmapItem* item = scene->addPixmap(gridPixmap);
                item->setPos(x, y);
                gridItems.push_back(item);
            }
        }
    }
    addGridItemsToScene();
}

void HexMap::drawUnits(std::vector<Unit> * Units)
{
    if(unitItems.empty())
    {
        for (std::vector<Unit>::iterator it = Units->begin(); it!= Units->end(); ++it)
            {
                int x = it->getCol() * xOffset;
                int y = it->getRow() * yOffset + (it->getCol() % 2) * (hexHeight / 2); // Versetzung für ungerade Spalten
                QGraphicsPixmapItem* item = scene->addPixmap(UnitType::getPixmap(it->getType()));
                item->setPos(x, y);
                unitItems.push_back(item);
            }

    }
    addUnitItemsToScene();
}

void HexMap::drawActiveMoveOverlay(int row_unit, int col_unit, int distance_unit)
{
    if(moveItems.empty())
    {
        for (int row = 0; row < height; ++row)
        {
            for (int col = 0; col < width; ++col)
            {
                if(distance(row,col,row_unit,col_unit)>0 and distance(row,col,row_unit,col_unit)<=distance_unit)
                {
                    int x = col * xOffset;
                    int y = row * yOffset + (col % 2) * (hexHeight / 2); // Versetzung für ungerade Spalten
                    QGraphicsPixmapItem* item = scene->addPixmap(movePixmap);
                    item->setPos(x, y);
                    moveItems.push_back(item);
                }
            }
        }
    }
    addMoveItemsToScene();
}


void HexMap::clearActiveMoveOverlay() {
    if (not moveItems.empty())
        {
        removeMoveItemsFromScene();
        moveItems.clear();
        }
}


void HexMap::removeHexItemsFromScene() {
    for (auto item : hexItems) {
        scene->removeItem(item);  // Entfernt jedes Item aus der Szene, löscht sie aber nicht
    }
}

void HexMap::addHexItemsToScene() {
    for (auto item : hexItems) {
        scene->addItem(item);  // Fügt die Items erneut zur Szene hinzu
    }
}
void HexMap::removeGridItemsFromScene()
{
    for (auto item : gridItems)
        {
            if (item->scene() == scene)
            {
                scene->removeItem(item);
            }
        }
}

void HexMap::addGridItemsToScene()
{
    for (auto item : gridItems)
    {
        if (item->scene() != scene) // Überprüfe, ob das Item bereits zur Szene gehört
        {
            scene->addItem(item);
        }
    }
}

void HexMap::removeMoveItemsFromScene()
{
    for (auto item : moveItems)
        {
            if (item->scene() == scene)
            {
                scene->removeItem(item);
            }
        }
}

void HexMap::addMoveItemsToScene()
{
    for (auto item : moveItems)
    {
        if (item->scene() != scene) // Überprüfe, ob das Item bereits zur Szene gehört
        {
            scene->addItem(item);
        }
    }
}




void HexMap::removeUnitItemsFromScene()
{
    for (auto item : unitItems)
        {
            if (item->scene() == scene)
            {
                scene->removeItem(item);
            }
        }
}

void HexMap::addUnitItemsToScene()
{
    for (auto item : unitItems)
    {
        if (item->scene() != scene) // Überprüfe, ob das Item bereits zur Szene gehört
        {
            scene->addItem(item);
        }
    }
}

void HexMap::setActiveOverlay(QGraphicsPixmapItem* overlayItem)
{
    clearActiveOverlay(); // Altes Overlay löschen
    activeOverlayItem = overlayItem;
    DrawActiveOverlay();
}

void HexMap::clearActiveOverlay() {
    if (activeOverlayItem)
        {
        if (activeOverlayItem->scene() != scene)
            {
            scene->removeItem(activeOverlayItem);
            }
        delete activeOverlayItem;
        activeOverlayItem = nullptr;
        }
}

void HexMap::DrawActiveOverlay()
{
    if (activeOverlayItem)
        if (activeOverlayItem->scene() != scene) // Überprüfe, ob das Item bereits zur Szene gehört
        {
            scene->addItem(activeOverlayItem);
        }
}

int HexMap::getWidth() const
{
    return width;
}
int HexMap::getHeight() const
{
    return height;
}
int HexMap::getXOffset() const
{
    return xOffset;
}
int HexMap::getYOffset() const
{
    return yOffset;
}
int HexMap::getHexHeight() const
{
    return hexHeight;
}
Hex& HexMap::getHex(int row, int col) {
    if (row >= 0 && row < height && col >= 0 && col < width) {
        return map[row][col];
    } else {
        throw std::out_of_range("HexMap::getHex: Index out of bounds.");
    }
}

int HexMap::distance(int row1, int col1, int row2, int col2)
{
    double q1=col1;
    double q2=col2;
    double r1=row1-((col1-(col1&1))/2);
    double r2=row2-((col2-(col2&1))/2);
    double s1=-q1-r1;
    double s2=-q2-r2;
    return (std::abs(q1-q2)+std::abs(r1-r2)+std::abs(s1-s2))/2;

}
