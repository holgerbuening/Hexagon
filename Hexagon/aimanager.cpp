#include "aimanager.h"
#include <algorithm>
#include "mainwindow.h"

AIManager::AIManager(MainWindow* mainWindow, HexMap* hexmap, std::vector<Unit>* units, const QString& currentPlayer, const QString& opponentPlayer)
    : mainWindow(mainWindow), hexmap(hexmap), units(units), currentPlayer(currentPlayer), opponentPlayer(opponentPlayer) 
{
enemyUnits = std::vector<Unit*>();
ownUnits = std::vector<Unit*>();
objectives = std::vector<Hex*>();
}

AIManager::~AIManager()
{
enemyUnits.clear();
ownUnits.clear();
objectives.clear();
}

void AIManager::processTurn()
{
    // Classify units
    classifyUnits();

    // Determine state for each unit
    determineState();

    // Perform actions based on determined states
    performActions();

    // Buy units based on available resources
    buyUnits();
}

void AIManager::classifyUnits()
{
    enemyUnits.clear();
    ownUnits.clear();
    objectives.clear();

    for (auto& unit : *units)
    {
        if (unit.getCountry() == opponentPlayer && unit.getType()!=UnitType::militarybase)
            enemyUnits.push_back(&unit);
        else if (unit.getCountry() == currentPlayer && unit.getType()!=UnitType::militarybase) 
            ownUnits.push_back(&unit);
        else if (unit.getType() == UnitType::militarybase && unit.getCountry() == opponentPlayer)
            objectives.push_back(&hexmap->getHex(unit.getRow(),unit.getCol()));
    }
    //search in hexmap for industries and cities and add them to objectives
    for (int row=0; row<hexmap->getHeight(); row++)
    {
        for (int col=0; col<hexmap->getWidth(); col++)
        {
            Hex& hex = hexmap->getHex(row,col);
            if (hex.getFieldType()==FieldType::City || hex.getFieldType()==FieldType::Industry)
            {
                objectives.push_back(&hex);
            }
        }
    }
}

void AIManager::determineState()
{
    int enemyCount = enemyUnits.size();
    int ownCount = ownUnits.size();
    int maxAttackUnits = ownCount / 2;
    if (maxAttackUnits==0)
    {
        maxAttackUnits=1;
    }
    int maxCaptureUnits = ownCount / 2;
    if (maxCaptureUnits==0)
    {
        maxCaptureUnits=1;
    }
    int attackUnits = 0;
    int captureUnits = 0;

    for (auto& unit : ownUnits)
    {
        if (unit->getType() == UnitType::medic)
        {
            unit->setAiState(NONE);
        }
        else if (unit->getCurrentState() < 30)
        {
            unit->setAiState(RETREAT);
        }
        else if (unitHoldsObjective(unit))
        {
            unit->setAiState(DEFEND);
        }
        else if (!objectives.empty() && captureUnits < maxCaptureUnits)
        {
            unit->setAiState(CAPTURE);
            captureUnits++;
        }
        else if (!enemyUnits.empty() && attackUnits < maxAttackUnits)
        {
            unit->setAiState(ATTACK);
            attackUnits++;
        }
        else
        {
            unit->setAiState(NONE);
        }
    }
}

//checks if the unit posseses an objective
bool AIManager::unitHoldsObjective(Unit* unit)
{
    for (auto& objective : objectives)
    {
        if (unit->getRow() == objective->getRow() && unit->getCol() == objective->getCol())
        {
            return true;
        }
    }
    return false;
}

void AIManager::performActions()
{
    for (auto& unit : ownUnits)
    {
        AIState state = unit->getAiState();

        switch (state)
        {
        case ATTACK:
            if (!enemyUnits.empty())
            {
                //sort enemy units by distance from unit
                std::sort(enemyUnits.begin(), enemyUnits.end(), [this, &unit](Unit* a, Unit* b)
                {
                    Hex aHex = hexmap->getHex(a->getRow(), a->getCol());
                    Hex bHex = hexmap->getHex(b->getRow(), b->getCol());
                    return hexmap->distance(unit->getRow(), unit->getCol(), aHex.getRow(), aHex.getCol()) < hexmap->distance(unit->getRow(), unit->getCol(), bHex.getRow(), bHex.getCol());
                });
                Unit* target = enemyUnits.front();
                Hex targetHex = hexmap->getHex(target->getRow(), target->getCol());

                if (hexmap->distance(unit->getRow(), unit->getCol(), targetHex.getRow(), targetHex.getCol()) <= unit->getAttackRange())
                {
                    engageCombat(unit, target);
                }
                else
                {
                    moveToTargetsNeighbour(unit, targetHex);
                }
            }
            break;

        case CAPTURE:
            if (!objectives.empty())
            {
                //sort objectives by distance from unit
                std::sort(objectives.begin(), objectives.end(), [this, &unit](const Hex* a, const Hex* b)
                {
                    return hexmap->distance(unit->getRow(), unit->getCol(), a->getRow(), a->getCol()) < hexmap->distance(unit->getRow(), unit->getCol(), b->getRow(), b->getCol());
                });

                
                Hex objectiveHex = *objectives.front();
                captureObjective(unit, objectiveHex);
            }
            break;

        case RETREAT:
            retreatToSafety(unit);
            break;

        case DEFEND:
            // Defensive action, stay in position
            break;
        }
    }
}

void AIManager::moveToTargetsNeighbour(Unit* unit, const Hex& targetHex)
{
    Hex startHex = hexmap->getHex(unit->getRow(), unit->getCol());
    int territory = startHex.getTerritory();
    //std::vector<Node> path = hexmap->AStar(startHex, targetHex, unit->getTerritory(), units);
    Hex target = hexmap->getClosestNeighbourSameTerritoryNoUnits(startHex,targetHex,territory,units);
    int attackRow = target.getRow();
    int attackCol = target.getCol();
    std::vector<Node> path = hexmap->AStar(startHex,hexmap->getHex(attackRow,attackCol),unit->getTerritory(), units);
    if (!path.empty())
    {
        Node nextMove = hexmap->getReachableNode(path, unit->getRemainingMovementPoints());
        mainWindow->moveUnit(unit,nextMove.row,nextMove.col);
        if (unit->getRemainingMovementPoints()<=0)
        {
            unit->setActed();
        }
    }
}

void AIManager::moveToTarget(Unit* unit, const Hex& targetHex)
{
    Hex startHex = hexmap->getHex(unit->getRow(), unit->getCol());
    int territory = startHex.getTerritory();
    //std::vector<Node> path = hexmap->AStar(startHex, targetHex, unit->getTerritory(), units);
   
    int attackRow = targetHex.getRow();
    int attackCol = targetHex.getCol();
    std::vector<Node> path = hexmap->AStar(startHex,hexmap->getHex(attackRow,attackCol),unit->getTerritory(), units);
    if (!path.empty())
    {
        Node nextMove = hexmap->getReachableNode(path, unit->getRemainingMovementPoints());
        mainWindow->moveUnit(unit,nextMove.row,nextMove.col);
        if (unit->getRemainingMovementPoints()<=0)
        {
            unit->setActed();
        }
    }
}




void AIManager::retreatToSafety(Unit* unit)
{
    // Logic to retreat to the safest position
}

void AIManager::captureObjective(Unit* unit, const Hex& objectiveHex)
{
    moveToTarget(unit, objectiveHex);
}

void AIManager::engageCombat(Unit* attacker, Unit* defender)
{
     if (mainWindow) {
        mainWindow->startCombat(*attacker, *defender);
        mainWindow->isAnybodyDead();
    }
}

void AIManager::buyUnits()
{
    //if no medic unit available and enough money, buy a medic unit
    if (!hasMedicUnit() 
        && mainWindow->getPlayerBalances(currentPlayer) >= UnitType::getPrice(UnitType::medic))
    {
        //find a free field next to own military base
        for (auto& unit : *units)
        {
            if (unit.getType() == UnitType::militarybase && unit.getCountry() == currentPlayer)
            {
                Hex baseHex = hexmap->getHex(unit.getRow(), unit.getCol());
                std::vector<Hex> neighbours = hexmap->getNeighborsSameTerritory(baseHex, baseHex.getTerritory());
                for (auto& neighbour : neighbours)
                {
                    if (isEmptyField(neighbour.getRow(), neighbour.getCol()))
                    {
                        Unit medic(UnitType::medic, neighbour.getRow(), neighbour.getCol(), currentPlayer);
                        units->push_back(medic);
                        
                        int newBalance =mainWindow->getPlayerBalances(currentPlayer) - 100;
                        break;
                    }
                }
            }
        }
    }

}

//check if a field is empty (no unit on it)
bool AIManager::isEmptyField(int row, int col)
{
    for (auto& unit : *units)
    {
        if (unit.getRow() == row && unit.getCol() == col)
        {
            return false;
        }
    }
    return true;
}

// write a function that checks if the player has a medic unit
bool AIManager::hasMedicUnit()
{
    bool hasMedic = false;
    for (auto& unit : ownUnits)
    {
        if (unit->getType() == UnitType::medic)
        {
            hasMedic= true;
        }
    }
    return hasMedic;
}


