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



#include "hexmap.h"
#include <QGraphicsPixmapItem>
#include <QGraphicsScene>
#include "hexitem.h"
#include <cstdlib> 
#include <random>
#include <ctime> 
#include <queue>
#include <unordered_map>
#include <functional>
#include <cmath>
#include <iostream>
#include <algorithm> //shuffle


HexMap::HexMap(int width, int height, std::unique_ptr<QGraphicsScene> externalScene):
    width(width),
    height(height),
    gridPixmap(":/hexfields/Images/grid_big.png"),
    movePixmap(":/hexfields/Images/grid_big_move.png"),
    scene(externalScene ? std::move(externalScene) : std::make_unique<QGraphicsScene>()),
    activeOverlayItem(nullptr)
{
    pixmapCountry1= QPixmap(":/Images/flag_lupony.png");
    pixmapCountry2= QPixmap(":/Images/flag_ursony.png");
    attackPixmap=QPixmap(":/hexfields/Images/grid_big_attack.png");
    pixmapShoulderboard00=QPixmap(":/Images/shoulderboards00.png").scaled(250,250,Qt::KeepAspectRatio,Qt::SmoothTransformation);
    pixmapShoulderboard01=QPixmap(":/Images/shoulderboards01.png").scaled(250,250,Qt::KeepAspectRatio,Qt::SmoothTransformation);
    pixmapShoulderboard02=QPixmap(":/Images/shoulderboards02.png").scaled(250,250,Qt::KeepAspectRatio,Qt::SmoothTransformation);
    pixmapShoulderboard03=QPixmap(":/Images/shoulderboards03.png").scaled(250,250,Qt::KeepAspectRatio,Qt::SmoothTransformation);
    pixmapShoulderboard04=QPixmap(":/Images/shoulderboards04.png").scaled(250,250,Qt::KeepAspectRatio,Qt::SmoothTransformation);
    pixmapShoulderboard05=QPixmap(":/Images/shoulderboards05.png").scaled(250,250,Qt::KeepAspectRatio,Qt::SmoothTransformation);
    pixmapShoulderboard06=QPixmap(":/Images/shoulderboards06.png").scaled(250,250,Qt::KeepAspectRatio,Qt::SmoothTransformation);
    pixmapShoulderboard07=QPixmap(":/Images/shoulderboards07.png").scaled(250,250,Qt::KeepAspectRatio,Qt::SmoothTransformation);
    pixmapShoulderboard08=QPixmap(":/Images/shoulderboards08.png").scaled(250,250,Qt::KeepAspectRatio,Qt::SmoothTransformation);
    pixmapShoulderboard09=QPixmap(":/Images/shoulderboards09.png").scaled(250,250,Qt::KeepAspectRatio,Qt::SmoothTransformation);
    pixmapShoulderboard10=QPixmap(":/Images/shoulderboards10.png").scaled(250,250,Qt::KeepAspectRatio,Qt::SmoothTransformation);

    //initialize map with Farmland
    map.resize(height, std::vector<Hex>(width));
    for (int y = 0; y < height; ++y) {
        map[y].resize(width);
        for (int x = 0; x < width; ++x) {
            map[y][x] = Hex(x, y, FieldType::Farmland); // example parameters
        }
    }

    //initialize hexItems
    //hexItems = {};

    //initialize gridItems
    gridItems = {};

    //initialize unitItems
    unitItems = {};

    //initialize flagItems
    flagItems = {};

    //initialize shoulderboardItems
    shoulderboardItems = {};

    //initialize stateItems
    stateItems = {};

    //initialize moveItems
    moveItems = {};

    //initialize attackItems
    attackItems = {};
}

void HexMap::resizeHexMap(int externalWidth, int externalHeight)
{
    width=externalWidth;
    height=externalHeight;
    map.resize(height, std::vector<Hex>(width));

    //initialize map with Farmland
    /*for (int y = 0; y < height; ++y) {
        map[y].resize(width);
        for (int x = 0; x < width; ++x) {
            map[y][x] = Hex(x, y, FieldType::Farmland); // example parameters
        }
    }*/
    qDebug() << "HexMap resized to " << width << "x" << height;
   
}

HexMap::~HexMap() {
    //qDebug() << "HexMap destructor";
    // release hexItems
    if (!hexItems.empty())
    {
    //qDebug() << "HexItems is not empty and will be deleted";
        for (auto hexItem : hexItems) {
            //delete hexItem;
        }
        hexItems.clear();
    }

    // release gridItems
    if (!gridItems.empty())
    {
        //qDebug() << "GridItems is not empty and will be deleted";
        for (auto item : gridItems) {
            delete item;
        }
    }

    // release unitItems
    if (!unitItems.empty())
    {
        //qDebug() << "UnitItems is not empty and will be deleted";
        for (auto item : unitItems) {
            delete item;
        }
    }

    // release flagItems
    if (!flagItems.empty())
    {
        //qDebug() << "FlagItems is not empty and will be deleted";
        for (auto item : flagItems) {
            delete item;
        }
    }

    // release stateItems
    if (!stateItems.empty())
    {
        //qDebug() << "StateItems is not empty and will be deleted";
        for (auto item : stateItems) {
            delete item;
        }
    }

    // release shoulderboardItems
    if (!shoulderboardItems.empty())
    {
        //qDebug() << "ShoulderboardItems is not empty and will be deleted";
        for (auto item : shoulderboardItems) {
            delete item;
        }
    }

    // release moveItems
    if (!moveItems.empty())
    {
        //qDebug() << "MoveItems is not empty and will be deleted";
        for (auto item : moveItems) {
            delete item;
        }
    }
    

    // release von attackItems
    if (!attackItems.empty())
    {
        //qDebug() << "AttackItems is not empty and will be deleted";
        for (auto item : attackItems) {
        delete item;
        }
    }
    // release roadItems
    if (!roadItems.empty())
    {
        //qDebug() << "RoadItems is not empty and will be deleted";
        for (auto item : roadItems) {
            delete item;
        }
    }

    // Releasing the activeOverlayItem
    //qDebug() << "activeOverlayItem is Smart Pointer and will be deleted automatically";

    // release scene
    //qDebug() << "Scene is Smart Pointer and will be deleted automatically";
     
    //qDebug() << "HexMap destructor finished";
   
}

void HexMap::createRandomMap()
{
    srand(static_cast<unsigned int>(time(nullptr)));

    //initialize map with Farmland
    for (int y = 0; y < height; ++y) {
        map[y].resize(width);
        for (int x = 0; x < width; ++x) {
            map[y][x] = Hex(x, y, FieldType::Farmland); // adjust example parameters
        }
    }


    // Create bigger lakes and mountain areas
    int addAreas1 = width * height / 80;//ocean
    int addAreas2 = width * height / 60;//mountain

    int randomNumberOfOceans = (rand() % 4)+addAreas1;
    int randomNumberMaxSizeOfOceans = (rand() % 8)+addAreas2;
    generateLargeAreas(FieldType::Ocean, randomNumberOfOceans, randomNumberMaxSizeOfOceans);
    int randomNumberOfMountainAreas = (rand() % 4)+addAreas1;
    int randomNumberMaxSizeOfMOuntainAreas = (rand() % 6)+addAreas2;
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
    }//end of random filling
    placeCities();
    placeIndustries();
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
    if (maxSize<4)
    {
        maxSize=4;
    }
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
                int y = row * yOffset + (col % 2) * (hexHeight / 2); // adjust for odd columns
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
                //create flag
                QGraphicsPixmapItem* flag;
                int x = it->getCol() * xOffset;
                int y = it->getRow() * yOffset + (it->getCol() % 2) * (hexHeight / 2); // adjust for odd columns
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

               
                //create unit
                QGraphicsPixmapItem* unititem = scene->addPixmap(UnitType::getPixmap(it->getType()));
                unititem->setPos(x, y);
                unitItems.push_back(unititem);

                //create state bar
                StateBarItem* stateItem = new StateBarItem(100, unititem);
                stateItem->setValue(it->getCurrentState());
                stateItem->setPos(300, 570);//in relation to the unititem
                stateItems.push_back(stateItem);

                 //create shoulderboard
                QGraphicsPixmapItem* shoulderboard;
                if (it->getType()!=UnitType::militarybase)
                {
                    switch (it->getExperience())
                    {
                        case 0:
                            shoulderboard = scene->addPixmap(pixmapShoulderboard00);
                            break;
                        case 1:
                            shoulderboard = scene->addPixmap(pixmapShoulderboard01);
                            break;
                        case 2:
                            shoulderboard = scene->addPixmap(pixmapShoulderboard02);
                            break;
                        case 3:
                            shoulderboard = scene->addPixmap(pixmapShoulderboard03);
                            break;
                        case 4:
                            shoulderboard = scene->addPixmap(pixmapShoulderboard04);
                            break;
                        case 5:
                            shoulderboard = scene->addPixmap(pixmapShoulderboard05);
                            break;
                        case 6:
                            shoulderboard = scene->addPixmap(pixmapShoulderboard06);
                            break;
                        case 7:
                            shoulderboard = scene->addPixmap(pixmapShoulderboard07);
                            break;
                        case 8:
                            shoulderboard = scene->addPixmap(pixmapShoulderboard08);
                            break;
                        case 9:
                            shoulderboard = scene->addPixmap(pixmapShoulderboard09);
                            break;
                        case 10:
                            shoulderboard = scene->addPixmap(pixmapShoulderboard10);
                            break;
                        default:
                            shoulderboard = scene->addPixmap(pixmapShoulderboard00);
                            break;
                    }
                    shoulderboard->setPos(x+550,y+150);
                    shoulderboardItems.push_back(shoulderboard);
                }

            }

    }
    addUnitItemsToScene();
    scene->update();
}

void HexMap::clearUnits()
{
    removeUnitItemsFromScene();
    unitItems.clear();
    flagItems.clear();
    stateItems.clear();
    shoulderboardItems.clear();
}

void HexMap::drawActiveMoveOverlay(int row_unit, int col_unit, int distance_unit, int territory_unit, std::vector<Unit> *Units)
{
bool otherUnit=false;
    if(moveItems.empty())
    {
        //limit loop to the area arount the unit
        int max_range=distance_unit+2;
        int min_row=row_unit-max_range;
        if (min_row<0)
        {
            min_row=0;
        }   
        int max_row=row_unit+max_range;
        if (max_row>height)
        {
            max_row=height;
        }
        int min_col=col_unit-max_range;
        if (min_col<0)
        {
            min_col=0;
        }
        int max_col=col_unit+max_range;
        if (max_col>width)
        {
            max_col=width;
        }
        for (int row = min_row; row < max_row; ++row)
        {
            for (int col = min_col; col < max_col; ++col)
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
                int actualMovementCost=calculateMovementCost(row_unit,col_unit,row,col,territory_unit,Units);
                if(distance(row,col,row_unit,col_unit)>0 && actualMovementCost<=distance_unit
                        && actualMovementCost!=-1
                        && (territory_unit==(FieldType::getTerritory(getHex(row,col).getFieldType())))&& !otherUnit)
                {
                    int x = col * xOffset;
                    int y = row * yOffset + (col % 2) * (hexHeight / 2); // adjust for odd columns
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
        if (attackRange==0) // if attackRange is 0, only the unit itself is highlighted "Building a road"
        {
            int x = col_unit * xOffset;
            int y = row_unit * yOffset + (col_unit % 2) * (hexHeight / 2); // adjust for odd columns
            QGraphicsPixmapItem* item = scene->addPixmap(attackPixmap);
            item->setPos(x, y);
            attackItems.push_back(item);
        }    
        else
        {
            //limit loop to the area arount the unit
            int max_range=attackRange+2;
            int min_row=row_unit-max_range;
            if (min_row<0)
            {
                min_row=0;
            }
            int max_row=row_unit+max_range;
            if (max_row>height)
            {
               max_row=height;
            }
            int min_col=col_unit-max_range;
            if (min_col<0)
            {
                min_col=0;
            }
            int max_col=col_unit+max_range;
            if (max_col>width)
            {
               max_col=width;
            }

            for (int row = min_row; row < max_row; ++row)
            {
                for (int col = min_col; col < max_col; ++col)
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
                
                int actualDistance = distance(row,col,row_unit,col_unit);
                if(actualDistance<=attackRange && attackUnit && actualDistance!=0)
                    {
                        int x = col * xOffset;
                        int y = row * yOffset + (col % 2) * (hexHeight / 2); // adjust for odd columns
                        QGraphicsPixmapItem* item = scene->addPixmap(attackPixmap);
                        item->setPos(x, y);
                        attackItems.push_back(item);
                    }
                    attackUnit=false;
                }
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
    for (auto item : hexItems) 
    {
        if (scene)
        {
            scene->removeItem(item);  // delete the items from the scene
        }
        delete item;
    }
    hexItems.clear();

}

void HexMap::addHexItemsToScene() {
    //qDebug() << "addHexItemsToScene" << scene.get();
    if(!hexItems.empty())
    {
        //qDebug() << "hexItems is not empty";
    
        for (auto item : hexItems) 
        {
            if (item->scene() != scene.get()) // check if the item already belongs to the scene
            {
                scene->addItem(item);  // add the items to the scene
            }
        }
    }
}

void HexMap::removeGridItemsFromScene()
{
    for (auto item : gridItems)
        {
            if (item->scene() == scene.get())
            {
                scene->removeItem(item);
            }
        }
}

void HexMap::addGridItemsToScene()
{
    for (auto item : gridItems)
    {
        if (item->scene() != scene.get()) // check if the item already belongs to the scene
        {
            scene->addItem(item);
        }
    }
}

void HexMap::removeMoveItemsFromScene()
{
    for (auto item : moveItems)
        {
            if (item->scene() == scene.get())
            {
                scene->removeItem(item);
            }
        }
}

void HexMap::addMoveItemsToScene()
{
    for (auto item : moveItems)
    {
        if (item->scene() != scene.get()) // check if the item already belongs to the scene
        {
            scene->addItem(item);
        }
    }
}

void HexMap::removeAttackItemsFromScene()
{
    for (auto item : attackItems)
        {
            if (item->scene() == scene.get())
            {
                scene->removeItem(item);
            }
        }
}

void HexMap::addAttackItemsToScene()
{
    for (auto item : attackItems)
    {
        if (item->scene() != scene.get()) // check if the item already belongs to the scene
        {
            scene->addItem(item);
        }
    }
}

void HexMap::removeUnitItemsFromScene()
{
    for (auto item : unitItems)
        {
            if (item->scene() == scene.get())
            {
                scene->removeItem(item);
            }
        }
    for (auto item : flagItems)
        {
            if (item->scene() == scene.get())
            {
                scene->removeItem(item);
            }
        }
    for (auto item : stateItems)
        {
            if (item->scene() == scene.get())
            {
                scene->removeItem(item);
            }
        }
    for (auto item : shoulderboardItems)
        {
            if (item->scene() == scene.get())
            {
                scene->removeItem(item);
            }
        }

}

void HexMap::addUnitItemsToScene()
{
    for (auto item : unitItems)
    {
        if (item->scene() != scene.get()) // check if the item already belongs to the scene
        {
            scene->addItem(item);
        }
    }
    for (auto item : flagItems)
    {
        if (item->scene() != scene.get()) // check if the item already belongs to the scene
        {
            scene->addItem(item);
        }
    }
    for (auto item : stateItems)
    {
        if (item->scene() != scene.get()) // check if the item already belongs to the scene
        {
            scene->addItem(item);
        }
    }
    for (auto item : shoulderboardItems)
    {
        if (item->scene() != scene.get()) // check if the item already belongs to the scene
        {
            scene->addItem(item);
        }
    }

}

void HexMap::setActiveOverlay(QGraphicsPixmapItem* overlayItem)
{
    clearActiveOverlay(); // delete the old overlay item
    activeOverlayItem.reset(overlayItem);
    DrawActiveOverlay();
}

void HexMap::clearActiveOverlay() {
    if (activeOverlayItem)
        {
        if (activeOverlayItem->scene() != scene.get())
            {
            scene->removeItem(activeOverlayItem.get());  // delete the item from the scene
            }
        //delete activeOverlayItem;
        activeOverlayItem = nullptr;
        }
}

void HexMap::DrawActiveOverlay()
{
    if (activeOverlayItem)
        if (activeOverlayItem->scene() != scene.get()) // check if the item already belongs to the scene
        {
            scene->addItem(activeOverlayItem.get());  // add the item to the scene
        }
}

QGraphicsScene* HexMap::getScene()
{
    return scene.get();
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

Hex& HexMap::getHex(int row, int col)
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
    return distance(a.getRow(),a.getCol(),b.getRow(),b.getCol());
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

Hex HexMap::getClosestNeighbourSameTerritoryNoUnits(Hex start, Hex target, int territory, std::vector<Unit> *units)
{
    int lowestRow;
    int lowestCol;
    std::vector<Hex> neighbors=getNeighborsSameTerritoryNoUnits(target, territory, units);
    if (!neighbors.empty())
    {
        int lowestDistance = calculateMovementCostStep2(start,neighbors[0],territory,units);
        lowestRow=neighbors[0].getRow();
        lowestCol=neighbors[0].getCol();
        for (std::vector<Hex>::iterator it = neighbors.begin();it !=neighbors.end();++it)
        {
            if (calculateMovementCostStep2(start,*it,territory,units)<lowestDistance)
            {
                lowestDistance=calculateMovementCostStep2(start,*it,territory,units);
                lowestRow=it->getRow();
                lowestCol=it->getCol();
            }
        }
        return getHex(lowestRow,lowestCol);
    }
    else
    {
        return start;
    }
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
    std::unordered_map<Hex, int, HashHex> gScore; // costs from start to this hex field
    std::unordered_map<Hex, int, HashHex> fScore; // estimated total costs (gScore + heuristic)

    openSet.emplace(0, start);
    gScore[start] = 0;
    fScore[start] = heuristic(start, goal);
    //std::cout <<"New calculateMovementCost\n Start: "<< start.getRow() <<"." << start.getCol() <<"\nGoal: "<< goal.getRow() <<"."<<goal.getCol()<<"\n";

    while (!openSet.empty()) {
        Hex current = openSet.top().second;
        //std::cout << "new current: "<< current.getRow() <<"."<<current.getCol()<<"\n";
        openSet.pop();

            if (current == goal) {
            return gScore[goal]; // cost from start to goal
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
                //std::cout << "new neigbor: "<< neighbor.getRow()<<"."<<neighbor.getCol()<<"\n";
                int tentative_gScore = gScore[current] + neighbor.getMovementCost();

                if (gScore.find(neighbor) == gScore.end() || tentative_gScore < gScore[neighbor])
                {
                    gScore[neighbor] = tentative_gScore;
                    fScore[neighbor] = tentative_gScore + heuristic(neighbor, goal);
                    openSet.emplace(fScore[neighbor], neighbor);
                    //std::cout << "neighbor in openSet aufgenommen\n";
                }
            }
        }
    }

    return -1; // did not find a path
}

std::vector<Node> HexMap::AStar(const Hex& start, const Hex& goal, int territory, std::vector<Unit>* units)
{
    std::priority_queue<std::pair<int, Hex>, std::vector<std::pair<int, Hex>>, std::greater<>> openSet;
    std::unordered_map<Hex, int, HashHex> gScore; // costs from start to this hex field
    std::unordered_map<Hex, int, HashHex> fScore; // estimated total costs (gScore + heuristic)

    //std::cout <<"\n\n\nNew Astar\n Start: "<< start.getRow() <<"." << start.getCol() <<"\nGoal: "<< goal.getRow() <<"."<<goal.getCol()<<"\n";
    openSet.emplace(0, start);
    gScore[start] = 0;
    fScore[start] = heuristic(start, goal);

    std::unordered_map<Hex, Hex, HashHex> cameFrom;

    while (!openSet.empty())
    {
        Hex current = openSet.top().second;
        openSet.pop();
        //std::cout << "new current: "<< current.getRow() <<"."<<current.getCol()<<"\n";//std::cout << "new current: "<< current.getRow() <<"."<<current.getCol()<<"\n";

        if (current == goal)
        {
            std::vector<Node> path;
            while (current.getRow() != start.getRow() || current.getCol()!= start.getCol())
            {
                path.push_back({current.getRow(), current.getCol(), gScore[current]});
                current = cameFrom[current];
            }
            path.push_back({start.getRow(), start.getCol(), gScore[start]});
            std::reverse(path.begin(), path.end());
            //std::cout << "Weg zurückgegeben\n";
            return path;
        }

        for (const Hex& neighbor : getNeighborsSameTerritoryNoUnits(current, territory, units))
        {
            //std::cout << "new neigbor: "<< neighbor.getRow()<<"."<<neighbor.getCol()<<"\n";
            int tentative_gScore = gScore[current] + neighbor.getMovementCost();


            if (gScore.find(neighbor) == gScore.end() || tentative_gScore < gScore[neighbor])
            {
                cameFrom[neighbor] = current;
                gScore[neighbor] = tentative_gScore;
                fScore[neighbor] = tentative_gScore + heuristic(neighbor, goal);
                openSet.emplace(fScore[neighbor], neighbor);
                //std::cout << "neighbor in openSet aufgenommen\n";
            }
        }
    }
    //std::cout << "did not find a path\n";
    return {}; // did not find a path
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
    std::unordered_map<Hex, int, HashHex> gScore; // costs from start to this hex field
    std::unordered_map<Hex, int, HashHex> fScore; // estimated total costs (gScore + heuristic)

    openSet.emplace(0, start);
    gScore[start] = 0;
    fScore[start] = heuristic(start, goal);

    while (!openSet.empty()) {
        Hex current = openSet.top().second;
        openSet.pop();

            if (current == goal) {
            return gScore[goal]; // cost from start to goal 
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

Node HexMap::getReachableNode(std::vector<Node>& path, int movementRange) {
    Node previousNode;
    previousNode={};
    for (Node& node : path)
    {
        if (node.cost > movementRange)
        {
            return previousNode;
            break;
        }
        previousNode=node;
    }
    return path.back(); // in case the last node is reachable
}

Unit* HexMap::getUnit(int row, int col, std::vector<Unit> *Units)
{
    Unit* returnUnit = nullptr;
    for (std::vector<Unit>::iterator it=Units->begin(); it!=Units->end();++it)
    {
        if(it->getRow()== row && it->getCol()==col)
        {
            returnUnit=&(*it);
            break;
        }
    }
    return returnUnit;
}

QDataStream& operator<<(QDataStream& out, const HexMap& hexMap)
{
    out << hexMap.width << hexMap.height;
    for (const auto& row : hexMap.map)
    {
        for (const auto& hex : row)
        {
            out << hex;
        }
    }
    return out;
}

QDataStream& operator>>(QDataStream& in, HexMap& hexMap)
{
    in >> hexMap.width >> hexMap.height;
    hexMap.map.resize(hexMap.height, std::vector<Hex>(hexMap.width));
    for (auto& row : hexMap.map)
    {
        for (auto& hex : row)
        {
            in >> hex;
        }
    }
    return in;
}

void HexMap::placeCities()
{
    int cityCount = width*height/80; // 1 city per 80 hexes
    std::random_device rd;
    std::mt19937 gen(rd());

    for (int i = 0; i < cityCount; ++i) {
        bool placed = false;
        while (!placed) {
            int row = gen() % height;
            int col = gen() % width;

            if (FieldType::getTerritory(getHex(row, col).getFieldType()) == 0) { // Ensure it's land
                getHex(row, col).setFieldType(FieldType::City);
                placed = true;
            }
        }
    }
    qDebug() <<cityCount << " Cities placed";
}

void HexMap::placeIndustries()
{
    int industryCount = width * height / 60; //1 industry per 60 hexes 
    std::random_device rd;
    std::mt19937 gen(rd());

    for (int i = 0; i < industryCount; ++i) {
        bool placed = false;
        while (!placed) {
            int row = gen() % height;
            int col = gen() % width;

            if (FieldType::getTerritory(getHex(row, col).getFieldType()) == 0 && FieldType::Type(getHex(row,col).getFieldType())!=FieldType::Type::City) { // Ensure it's land
                getHex(row, col).setFieldType(FieldType::Industry);
                placed = true;
            }
        }
    }
    qDebug() <<industryCount << " Industries placed";
}

void HexMap::clearRoads() {
    for (auto item : roadItems) {
        scene->removeItem(item);
        delete item;
    }
    roadItems.clear();
}

int HexMap::determineDirection(const Hex& from, const Hex& to) {
  int dRow = to.getRow() - from.getRow();
    int dCol = to.getCol() - from.getCol();
    bool isOddCol = (from.getCol() % 2 == 1);

    if (isOddCol) {
        if (dRow == -1 && dCol == 0 ) return 0; // top
        if (dRow == 0 && dCol == +1) return 1; // top right
        if (dRow == +1  && dCol == +1) return 2; // bottom right 
        if (dRow == +1 && dCol == 0) return 3; // bottom
        if (dRow == +1 && dCol == -1 ) return 4; // bottom left
        if (dRow == 0  && dCol == -1) return 5; // top left
        
    } else {
        if (dRow == -1 && dCol == 0 ) return 0; // top
        if (dRow == -1  && dCol == +1) return 1; // top right
        if (dRow == 0 && dCol == +1 ) return 2; // bottom right
        if (dRow == +1 && dCol == 0) return 3; // bottom
        if (dRow == 0  && dCol == -1) return 4; // bottom left
        if (dRow == -1 && dCol == -1) return 5; // top left
    }

    return -1; // Kein Nachbar   
}

void HexMap::drawRoads() {
    clearRoads();
    for (int row = 0; row < height; ++row) {
        for (int col = 0; col < width; ++col) {
            const Hex& hex = getHex(row, col);
            if (hex.getHasRoad()) {
                int x = col * xOffset;
                int y = row * yOffset + (col % 2) * (hexHeight / 2); // adjust for odd columns
                
                QGraphicsPixmapItem* roadCenter = scene->addPixmap(QPixmap(":/Images/road_center.png"));
                roadCenter->setPos(x, y);
                roadItems.push_back(roadCenter);

                // Nachbarverbindungen prüfen
                for (Hex neighbor : getNeighbors(hex)) {
                    if (neighbor.getHasRoad()) {
                        int direction = determineDirection(hex, neighbor); 
                        QGraphicsPixmapItem* connector = scene->addPixmap(QPixmap(":/Images/road_" + QString::number(direction) + ".png"));
                        connector->setPos(x , y );
                        roadItems.push_back(connector);
                    }
                }
            }
        }
    }
}
