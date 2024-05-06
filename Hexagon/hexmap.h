#ifndef HEXMAP_H
#define HEXMAP_H

#include <vector>
#include <QGraphicsScene>
#include <QPixmap>
#include <QGraphicsPixmapItem>
#include "hex.h"
#include "fieldtype.h"
#include "hexitem.h"

class HexMap {
public:
    HexMap(int width, int height,QGraphicsScene* scene );
    void createRandomMap();
    void drawMap();
    void drawGrid();
    void setActiveOverlay(QGraphicsPixmapItem* overlayItem);
    void DrawActiveOverlay();
    void clearActiveOverlay();
    void removeHexItemsFromScene();
    void addHexItemsToScene();
    void removeGridItemsFromScene();
    void addGridItemsToScene();
    Hex& getHex(int row, int col);
    int getWidth() const;
    int getHeight() const;
    int getXOffset() const;
    int getYOffset() const;
    int getHexHeight() const;
     std::vector<HexItem*> hexItems;  // Vector von HexItem-Zeigern

private:
    QGraphicsPixmapItem* activeOverlayItem = nullptr;
    std::vector<std::vector<Hex>> map;
    std::vector<QGraphicsPixmapItem*> gridItems;
    QGraphicsScene* scene = nullptr;
    int width, height;
    const int hexWidth = 900; // Breite der Hex-Zelle
    const int hexHeight = 600; // Höhe der Hex-Zelle
    const int xOffset = 600;// * 0.75;
    const int yOffset = 600;
    QPixmap gridPixmap;

};



#endif // HEXMAP_H
