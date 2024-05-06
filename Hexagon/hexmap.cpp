#include "hexmap.h"
#include <QGraphicsPixmapItem>
#include <QGraphicsScene>
#include "hexitem.h"
#include <cstdlib> // Für rand() und srand()
#include <ctime> // Für time()

HexMap::HexMap(int width, int height, QGraphicsScene* scene_v)
 : width(width), height(height),  gridPixmap(":/hexfields/Images/grid_big.png")
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

