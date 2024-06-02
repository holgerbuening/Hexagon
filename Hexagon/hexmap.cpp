#include "hexmap.h"
#include <QGraphicsPixmapItem>
#include <QGraphicsScene>
#include "hexitem.h"
#include <cstdlib> // Für rand() und srand()
#include <random>
#include <ctime> // Für time()
#include <queue>
#include <unordered_map>
#include <functional>
#include <cmath>
#include <iostream>
#include <algorithm> //shuffle


HexMap::HexMap(int width, int height, QGraphicsScene* scene_v)
 : width(width), height(height),  gridPixmap(":/hexfields/Images/grid_big.png"), movePixmap(":/hexfields/Images/grid_big_move.png")
 {
    pixmapCountry1= QPixmap(":/Images/flag_lupony.png");
    pixmapCountry2= QPixmap(":/Images/flag_ursony.png");
    attackPixmap=QPixmap(":/hexfields/Images/grid_big_attack.png");
    //activeOverlayItem=new QGraphicsPixmapItem(nullptr);
    activeOverlayItem=nullptr;
    scene=scene_v;
    map.resize(height);
    for (int y = 0; y < height; ++y) {
        map[y].resize(width);
        for (int x = 0; x < width; ++x) {
            map[y][x] = Hex(x, y, FieldType::Farmland); // Beispielparameter anpassen
        }
    }
}

HexMap::~HexMap() {
    // Freigeben von hexItems
    for (auto hexItem : hexItems) {
        delete hexItem;
    }

    // Freigeben von gridItems
    for (auto item : gridItems) {
        delete item;
    }

    // Freigeben von unitItems
    for (auto item : unitItems) {
        delete item;
    }

    // Freigeben von flagItems
    for (auto item : flagItems) {
        delete item;
    }

    // Freigeben von moveItems
    for (auto item : moveItems) {
        delete item;
    }

    // Freigeben von attackItems
    for (auto item : attackItems) {
        delete item;
    }

    // Freigeben des activeOverlayItem, falls vorhanden
    /*if (activeOverlayItem!=nullptr)
    {
        delete activeOverlayItem;
        activeOverlayItem=nullptr;
    }*/

    // Keine Notwendigkeit, die QGraphicsScene freizugeben, da sie nicht von HexMap erstellt wurde
    // und vermutlich woanders verwaltet wird
}



void HexMap::createRandomMap()
{
    srand(static_cast<unsigned int>(time(nullptr)));

    //initialize map with Farmland
    for (int y = 0; y < height; ++y) {
        map[y].resize(width);
        for (int x = 0; x < width; ++x) {
            map[y][x] = Hex(x, y, FieldType::Farmland); // Beispielparameter anpassen
        }
    }


     // Create bigger lakes and mountain areas
     int randomNumberOfOceans = (rand() % 4)+3;
     int randomNumberMaxSizeOfOceans = (rand() % 8)+4;
     generateLargeAreas(FieldType::Ocean, randomNumberOfOceans, randomNumberMaxSizeOfOceans);
     int randomNumberOfMountainAreas = (rand() % 4)+3;
     int randomNumberMaxSizeOfMOuntainAreas = (rand() % 6)+4;
     generateLargeAreas(FieldType::Mountain, randomNumberOfMountainAreas, randomNumberMaxSizeOfMOuntainAreas);

     // Rest of the map randomly filled
     for (int y = 0; y < height; ++y) {
         for (int x = 0; x < width; ++x) {
             if (map[y][x].getFieldType() == FieldType::Farmland) {
                 int randomType = rand() % 100;
                 if (randomType < 25)
                 {
                     map[y][x].setFieldType(FieldType::Hills);
                 }
                 else if (randomType < 70 )
                 {
                     map[y][x].setFieldType(FieldType::Woods);
                 }
                 map[y][x].setMovementCost();
                 map[y][x].setDefense(FieldType::getDefense(map[y][x].getFieldType()));
             }
         }
     }
 }

void HexMap::generateLargeAreas(FieldType::Type type, int numAreas, int maxSize)
{
    for (int i = 0; i < numAreas; ++i) {
        int startX = rand() % width;
        int startY = rand() % height;
        floodFill(startX, startY, type, maxSize);
    }
}

void HexMap::floodFill(int startX, int startY, FieldType::Type type, int maxSize)
{
    std::vector<std::pair<int, int>> stack;
    stack.push_back({startX, startY});
    int size = 0;
    int sizeOfThisArea = (rand() % (maxSize-3))+4;
    while (!stack.empty() && size < sizeOfThisArea)
    {
        auto [x, y] = stack.back();
        stack.pop_back();

        if (isValidPosition(y, x) && map[y][x].getFieldType() == FieldType::Farmland)
        {
            map[y][x].setFieldType(type);
            map[y][x].setMovementCost();
            map[y][x].setDefense(FieldType::getDefense(type));
            ++size;

            std::vector<Hex> neighbours= getNeighbors(getHex(y,x));
            std::random_device rd;
            std::mt19937 g(rd());
            std::shuffle(neighbours.begin(), neighbours.end(), g );
            for (std::vector<Hex>::iterator it=neighbours.begin();it != neighbours.end();++it)
            {
                stack.push_back({it->getCol(),it->getRow()});

            }
        }
    }
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
    clearUnits();
    if(unitItems.empty())
    {
        for (std::vector<Unit>::iterator it = Units->begin(); it!= Units->end(); ++it)
            {
                QGraphicsPixmapItem* flag;
                int x = it->getCol() * xOffset;
                int y = it->getRow() * yOffset + (it->getCol() % 2) * (hexHeight / 2); // Versetzung für ungerade Spalten
                if (it->getCountry()=="Lupony")
                {
                    flag = scene->addPixmap(pixmapCountry1);
                }
                else
                {
                    flag = scene->addPixmap(pixmapCountry2);
                }
                flag->setPos(x+250,y);
                flagItems.push_back(flag);
                QGraphicsPixmapItem* item = scene->addPixmap(UnitType::getPixmap(it->getType()));
                item->setPos(x, y);
                unitItems.push_back(item);

            }

    }
    addUnitItemsToScene();
}

void HexMap::clearUnits()
{
    removeUnitItemsFromScene();
    unitItems.clear();
    flagItems.clear();
}

void HexMap::drawActiveMoveOverlay(int row_unit, int col_unit, int distance_unit, int territory_unit, std::vector<Unit> *Units)
{
bool otherUnit=false;
    if(moveItems.empty())
    {
        for (int row = 0; row < height; ++row)
        {
            for (int col = 0; col < width; ++col)
            {
                //check if there is an unit on target already
                for (std::vector<Unit>::iterator it = Units->begin(); it!= Units->end(); ++it)
                {
                    //there is already an unit
                    if (it->getCol()==col && it->getRow()==row) //there is already an unit
                    {
                    otherUnit=true;
                    }
                }

                if(distance(row,col,row_unit,col_unit)>0 && calculateMovementCost(row_unit,col_unit,row,col,territory_unit,Units)<=distance_unit
                        && (territory_unit==(FieldType::getTerritory(getHex(row,col).getFieldType())))&& !otherUnit)
                {
                    int x = col * xOffset;
                    int y = row * yOffset + (col % 2) * (hexHeight / 2); // Versetzung für ungerade Spalten
                    QGraphicsPixmapItem* item = scene->addPixmap(movePixmap);
                    item->setPos(x, y);
                    moveItems.push_back(item);
                }
                otherUnit=false;
            }
        }
    }
    addMoveItemsToScene();
}


void HexMap::clearActiveMoveOverlay() {
    if (!moveItems.empty())
        {
        removeMoveItemsFromScene();
        moveItems.clear();
        }
}

void HexMap::drawActiveAttackOverlay(int row_unit, int col_unit, int attackRange, QString opponent, std::vector<Unit> *Units)
{
bool attackUnit=false;
    if(attackItems.empty())
    {
        for (int row = 0; row < height; ++row)
        {
            for (int col = 0; col < width; ++col)
            {
                //check if there is an unit on target
                for (std::vector<Unit>::iterator it = Units->begin(); it!= Units->end(); ++it)
                {
                    //there is an opponent unit
                    if (it->getCol()==col && it->getRow()==row && it->getCountry()==opponent) //there is already an unit
                    {
                    attackUnit=true;
                    }
                }

                if(distance(row,col,row_unit,col_unit)<=attackRange && attackUnit)
                {
                    int x = col * xOffset;
                    int y = row * yOffset + (col % 2) * (hexHeight / 2); // Versetzung für ungerade Spalten
                    QGraphicsPixmapItem* item = scene->addPixmap(attackPixmap);
                    item->setPos(x, y);
                    attackItems.push_back(item);
                }
                attackUnit=false;
            }
        }
    }
    addAttackItemsToScene();
}


void HexMap::clearActiveAttackOverlay() {
    if (!attackItems.empty())
        {
        removeAttackItemsFromScene();
        attackItems.clear();
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

void HexMap::removeAttackItemsFromScene()
{
    for (auto item : attackItems)
        {
            if (item->scene() == scene)
            {
                scene->removeItem(item);
            }
        }
}

void HexMap::addAttackItemsToScene()
{
    for (auto item : attackItems)
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
    for (auto item : flagItems)
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
    for (auto item : flagItems)
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
const Hex& HexMap::getHex(int row, int col) const
{
    if (row >= 0 && row < height && col >= 0 && col < width)
    {
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

int HexMap::heuristic(const Hex &a, const Hex &b) {
    // Manhattan-Abstand als Heuristik
    //return std::abs(a.getRow() - b.getRow()) + std::abs(a.getCol() - b.getCol());
    return distance(a.getRow(),a.getCol(),b.getRow(),a.getCol());
}

std::vector<Hex> HexMap::getNeighbors(const Hex &hex)
{
    std::vector<Hex> neighbors;
    for (int row = 0; row < height; ++row)
    {
        for (int col = 0; col < width; ++col)
        {
            if(distance(row,col,hex.getRow(),hex.getCol())==1)
            {
                neighbors.push_back(getHex(row, col));
            }
        }
    }
    return neighbors;

}

std::vector<Hex> HexMap::getNeighborsSameTerritory(const Hex &hex, int territory)
{
    std::vector<Hex> neighbors=getNeighbors(hex);
    for (std::vector<Hex>::iterator it = neighbors.begin(); it!= neighbors.end(); )
    {
        if (FieldType::getTerritory(it->getFieldType())!=territory)
        {
            it=neighbors.erase(it);
        }
        else
        {
            ++it;
        }
    }
    return neighbors;
}

std::vector<Hex> HexMap::getNeighborsSameTerritoryNoUnits(const Hex &hex, int territory, std::vector<Unit> *units)
{
bool otherUnit=false;
    std::vector<Hex> neighbors=getNeighbors(hex);
    for (std::vector<Hex>::iterator it = neighbors.begin(); it!= neighbors.end(); )
    {
        for (std::vector<Unit>::iterator itUnit = units->begin(); itUnit!= units->end(); ++itUnit)
        {
            //there is already an unit
            if (itUnit->getCol()==it->getCol() && itUnit->getRow()==it->getRow()) //there is already an unit
            {
            otherUnit=true;
            }
        }
        if (FieldType::getTerritory(it->getFieldType())!=territory || otherUnit)
        {
            it=neighbors.erase(it);
        }
        else
        {
            ++it;
        }
        otherUnit=false;
    }


    return neighbors;
}

bool HexMap::isValidPosition(int row, int col)const
{
    bool returnwert=true;
    if (row>=height || row<0 || col >=width || col <0)
    {
        returnwert = false;
    }
    return returnwert;
}

int HexMap::calculateMovementCostStep2(const Hex &start, const Hex &goal, int territory, std::vector<Unit>*units) {
    std::priority_queue<std::pair<int, Hex>, std::vector<std::pair<int, Hex>>, std::greater<>> openSet;
    std::unordered_map<Hex, int, HashHex> gScore; // Kosten vom Start bis zu diesem Hex-Feld
    std::unordered_map<Hex, int, HashHex> fScore; // geschätzte Gesamtkosten (gScore + Heuristik)

    openSet.emplace(0, start);
    gScore[start] = 0;
    fScore[start] = heuristic(start, goal);

    while (!openSet.empty()) {
        Hex current = openSet.top().second;
        openSet.pop();

            if (current == goal) {
            return gScore[goal]; // Kosten bis zum Ziel
        }

        if (territory==99)
        {
            for (const Hex &neighbor : getNeighborsSameTerritoryNoUnits(current,territory,units))
            {
                int tentative_gScore = gScore[current] + neighbor.getMovementCost();

                if (gScore.find(neighbor) == gScore.end() || tentative_gScore < gScore[neighbor])
                {
                    gScore[neighbor] = tentative_gScore;
                    fScore[neighbor] = tentative_gScore + heuristic(neighbor, goal);
                    openSet.emplace(fScore[neighbor], neighbor);
                }
            }

        }
        else
        {
            for (const Hex &neighbor : getNeighborsSameTerritoryNoUnits(current,territory,units))
            {
                int tentative_gScore = gScore[current] + neighbor.getMovementCost();

                if (gScore.find(neighbor) == gScore.end() || tentative_gScore < gScore[neighbor])
                {
                    gScore[neighbor] = tentative_gScore;
                    fScore[neighbor] = tentative_gScore + heuristic(neighbor, goal);
                    openSet.emplace(fScore[neighbor], neighbor);
                }
            }
        }
    }

    return -1; // Kein Weg gefunden
}

int HexMap::calculateMovementCost(int startRow, int startCol, int goalRow, int goalCol, int territory, std::vector<Unit> *units)
{
    const Hex &start = getHex(startRow,startCol);
    const Hex &goal = getHex(goalRow, goalCol);
    return calculateMovementCostStep2(start, goal, territory, units);
}

int HexMap::calculateMovementCost(int startRow, int startCol, int goalRow, int goalCol, int territory)
{
    const Hex &start = getHex(startRow,startCol);
    const Hex &goal = getHex(goalRow, goalCol);
    return calculateMovementCostStep2(start, goal, territory);
}

int HexMap::calculateMovementCostStep2(const Hex &start, const Hex &goal, int territory)
{
    std::priority_queue<std::pair<int, Hex>, std::vector<std::pair<int, Hex>>, std::greater<>> openSet;
    std::unordered_map<Hex, int, HashHex> gScore; // Kosten vom Start bis zu diesem Hex-Feld
    std::unordered_map<Hex, int, HashHex> fScore; // geschätzte Gesamtkosten (gScore + Heuristik)

    openSet.emplace(0, start);
    gScore[start] = 0;
    fScore[start] = heuristic(start, goal);

    while (!openSet.empty()) {
        Hex current = openSet.top().second;
        openSet.pop();

            if (current == goal) {
            return gScore[goal]; // Kosten bis zum Ziel
        }

        if (territory==99)
        {
            for (const Hex &neighbor : getNeighborsSameTerritory(current,territory))
            {
                int tentative_gScore = gScore[current] + neighbor.getMovementCost();

                if (gScore.find(neighbor) == gScore.end() || tentative_gScore < gScore[neighbor])
                {
                    gScore[neighbor] = tentative_gScore;
                    fScore[neighbor] = tentative_gScore + heuristic(neighbor, goal);
                    openSet.emplace(fScore[neighbor], neighbor);
                }
            }

        }
        else
        {
            for (const Hex &neighbor : getNeighborsSameTerritory(current,territory))
            {
                int tentative_gScore = gScore[current] + neighbor.getMovementCost();

                if (gScore.find(neighbor) == gScore.end() || tentative_gScore < gScore[neighbor])
                {
                    gScore[neighbor] = tentative_gScore;
                    fScore[neighbor] = tentative_gScore + heuristic(neighbor, goal);
                    openSet.emplace(fScore[neighbor], neighbor);
                }
            }
        }
    }

    return -1; // Kein Weg gefunden
}
