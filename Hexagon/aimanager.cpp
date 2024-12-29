#include "aimanager.h"
#include <algorithm>
#include "mainwindow.h"

AIManager::AIManager(MainWindow* mainWindow, HexMap* hexmap, std::vector<Unit>* units, const QString& currentPlayer, const QString& opponentPlayer)
    : mainWindow(mainWindow), hexmap(hexmap), units(units), currentPlayer(currentPlayer), opponentPlayer(opponentPlayer) 
{

}

void AIManager::processTurn()
{
    // Classify units
    classifyUnits();

    // Determine state for each unit
    determineState();

    // Perform actions based on determined states
    performActions();
}

void AIManager::classifyUnits()
{
    enemyUnits.clear();
    ownUnits.clear();
    objectives.clear();

    for (auto& unit : *units)
    {
        if (unit.getCountry() == opponentPlayer)
            enemyUnits.push_back(&unit);
        else if (unit.getCountry() == currentPlayer) // exclude military bases from ownUnits
            ownUnits.push_back(&unit);
        else if (unit.getType() == UnitType::militarybase)
            objectives.push_back(&unit);
        //HQ needs to be added to objectives and not Attack targets same with cities and industries
    }
}

void AIManager::determineState()
{
    int enemyCount = enemyUnits.size();
    int ownCount = ownUnits.size();
    int maxAttackUnits = ownCount / 2;
    int maxCaptureUnits = ownCount / 2;
    int attackUnits = 0;
    int captureUnits = 0;

    for (auto& unit : ownUnits)
    {
        if (unit->getCurrentState() < 30)
        {
            unit->setAiState(RETREAT);
        }
        else if (!enemyUnits.empty() && attackUnits < maxAttackUnits)
        {
            unit->setAiState(ATTACK);
            attackUnits++;
        }
        else if (!objectives.empty() && captureUnits < maxCaptureUnits)
        {
            unit->setAiState(CAPTURE);
            captureUnits++;
        }
        else
        {
            unit->setAiState(DEFEND);
        }
    }
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
                Unit* target = enemyUnits.front();
                Hex targetHex = hexmap->getHex(target->getRow(), target->getCol());

                if (hexmap->distance(unit->getRow(), unit->getCol(), targetHex.getRow(), targetHex.getCol()) <= unit->getAttackRange())
                {
                    engageCombat(unit, target);
                }
                else
                {
                    moveToTarget(unit, targetHex);
                }
            }
            break;

        case CAPTURE:
            if (!objectives.empty())
            {
                Hex objectiveHex = hexmap->getHex(objectives.front()->getRow(), objectives.front()->getCol());
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

void AIManager::moveToTarget(Unit* unit, const Hex& targetHex)
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
