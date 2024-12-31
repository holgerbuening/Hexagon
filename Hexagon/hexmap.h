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


#ifndef HEXMAP_H
#define HEXMAP_H

#include <vector>
#include <QGraphicsScene>
#include <QPixmap>
#include <QGraphicsPixmapItem>
#include "hex.h"
#include "fieldtype.h"
#include "hexitem.h"
#include "unit.h"
#include <QDataStream>
#include <memory>

struct Node
{
    int row, col, cost;
    bool operator>(const Node& other) const
    {
        return cost > other.cost;
    }
    bool operator<(const Node& other)const
    {
        return cost < other.cost;
    }
    bool operator==(const Node& other) const
    {
        return cost == other.cost;
    }
};


class HexMap {
public:
    HexMap(int width, int height,std::unique_ptr<QGraphicsScene> esternalScene = nullptr);
    ~HexMap();
    void resizeHexMap(int width, int height);
    void createRandomMap();
    void drawGrid();
    void drawUnits(std::vector<Unit>*);
    void clearUnits();
    void setActiveOverlay(QGraphicsPixmapItem* overlayItem);
    void DrawActiveOverlay();
    void clearActiveOverlay();
    void drawActiveMoveOverlay(int row, int col, int distance, int territory_unit, std::vector<Unit>* units);
    void clearActiveMoveOverlay();
    void drawActiveAttackOverlay(int row, int col, int attackRange, QString opponent, std::vector<Unit>* units);
    void clearActiveAttackOverlay();
    void removeHexItemsFromScene();
    void addHexItemsToScene();
    void removeGridItemsFromScene();
    void addGridItemsToScene();
    Node getReachableNode(std::vector<Node>& path, int movementRange);
    QGraphicsScene* getScene();
    Hex& getHex(int row, int col);
    int getWidth() const;
    int getHeight() const;
    int getXOffset() const;
    int getYOffset() const;
    int getHexHeight() const;
    int heuristic(const Hex &a, const Hex &b);
    Unit* getUnit(int row, int col, std::vector<Unit>* Units);
    std::vector<Hex> getNeighbors(const Hex &hex);
    std::vector<Hex> getNeighborsSameTerritory(const Hex &hex, int territory);
    std::vector<Hex> getNeighborsSameTerritoryNoUnits(const Hex &hex, int territory, std::vector<Unit>*units);
    Hex getClosestNeighbourSameTerritoryNoUnits(Hex start, Hex target, int territory, std::vector<Unit>*units); //returns the closest neighbour of target to get to from start
    int calculateMovementCostStep2(const Hex &start, const Hex &goal, int territory, std::vector<Unit>*units);
    int calculateMovementCostStep2(const Hex &start, const Hex &goal, int territory);
    std::vector<Node> AStar(const Hex& start, const Hex& goal, int territory, std::vector<Unit>*units);
    int calculateMovementCost(int startRow, int startCol, int goalRow, int goalCol, int territory,std::vector<Unit>*units);
    int calculateMovementCost(int startRow, int startCol, int goalRow, int goalCol, int territory);
    std::vector<HexItem*> hexItems;  // Vector von HexItem-Zeigern
    static int distance(int row1, int col1, int row2, int col2);

    friend QDataStream& operator<<(QDataStream& out, const HexMap& hexMap);
    friend QDataStream& operator>>(QDataStream& in, HexMap& hexMap);

private:
    std::unique_ptr<QGraphicsPixmapItem> activeOverlayItem = nullptr;
    std::vector<std::vector<Hex>> map;
    std::vector<QGraphicsPixmapItem*> gridItems;
    std::vector<QGraphicsPixmapItem*> unitItems;
    std::vector<QGraphicsPixmapItem*> flagItems;
    std::vector<QGraphicsPixmapItem*> moveItems;
    std::vector<QGraphicsPixmapItem*> attackItems;
    std::unique_ptr<QGraphicsScene> scene;
    int width, height;
    const int hexWidth = 900; // width of the hex cell
    const int hexHeight = 600; //height of the hex cell
    const int xOffset = 600;// * 0.75;
    const int yOffset = 600;
    QPixmap gridPixmap;
    QPixmap movePixmap;
    QPixmap attackPixmap;
    QPixmap pixmapCountry1;
    QPixmap pixmapCountry2;
    void removeMoveItemsFromScene();
    void addMoveItemsToScene();
    void removeAttackItemsFromScene();
    void addAttackItemsToScene();
    void addUnitItemsToScene();
    void removeUnitItemsFromScene();
    bool isValidPosition(int row, int col)const;
    void generateLargeAreas(FieldType::Type type, int numAreas, int maxSize);
    void floodFill(int startX, int startY, FieldType::Type type, int maxSize);
    void placeCities();
    void placeIndustries();
};

QDataStream& operator<<(QDataStream& out, const HexMap& hexMap);
QDataStream& operator>>(QDataStream& in, HexMap& hexMap);

#endif // HEXMAP_H
