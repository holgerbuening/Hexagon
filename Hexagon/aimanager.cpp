#include "aimanager.h"
#include <algorithm>
#include "mainwindow.h"

AIManager::AIManager(MainWindow* mainWindow, HexMap* hexmap, std::vector<Unit>* units, const QString& currentPlayer, const QString& opponentPlayer)
    : mainWindow(mainWindow), hexmap(hexmap), units(units), currentPlayer(currentPlayer), opponentPlayer(opponentPlayer) 
{
enemyUnits = std::vector<Unit*>();
ownUnits = std::vector<Unit*>();
objectives = std::vector<Hex*>();
ownFreeUnits = std::vector<Unit*>();
CitiesAndIndustries = std::vector<Hex*>();
//fill cities and industries
for (int row=0; row<hexmap->getHeight(); row++)
    {
        for (int col=0; col<hexmap->getWidth(); col++)
        {
            Hex& hex = hexmap->getHex(row,col);
            if (hex.getFieldType()==FieldType::City || hex.getFieldType()==FieldType::Industry)
            {
                CitiesAndIndustries.push_back(&hex);
            }
        }
    }
}

AIManager::~AIManager()
{
enemyUnits.clear();
ownUnits.clear();
objectives.clear();
ownFreeUnits.clear();
CitiesAndIndustries.clear();
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

    // Update the graphics view
    hexmap->clearUnits();
    hexmap->drawUnits(units);
    mainWindow->GraphicsViewUpdate();
}

void AIManager::classifyUnits()
{
    enemyUnits.clear();
    ownUnits.clear();
    ownFreeUnits.clear();
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
    //fill ownFreeUnits with units that in ownUnits and have aiState NONE
    for (auto& unit : ownUnits)
    {
        if (unit->getAiState()==NONE)
        {
            ownFreeUnits.push_back(unit);
        }
    }

    //search in hexmap for industries and cities and add them to objectives
    for (int row=0; row<hexmap->getHeight(); row++)
    {
        for (int col=0; col<hexmap->getWidth(); col++)
        {
            Hex& hex = hexmap->getHex(row,col);
            if (hex.getFieldType()==FieldType::City || hex.getFieldType()==FieldType::Industry)
            {
                //if field is reachable add it to objectives
                if (isReachable(hexmap->getHex(ownUnits.front()->getRow(),ownUnits.front()->getCol()),hex))
                {
                    //if field is empty add it to objectives
                    if (isEmptyField(row,col))
                    {
                        objectives.push_back(&hex);
                    }
                    //if field is not empty, check if it is an enemy unit
                    else
                    {
                        for (auto& unit : *units)
                        {
                            if (unit.getRow()==row && unit.getCol()==col && unit.getCountry()==opponentPlayer)
                            {
                                objectives.push_back(&hex);
                            }
                        }
                    }
                }
            }
        }
    }
}

void AIManager::determineState()
{
    int enemyCount = enemyUnits.size();
    int ownCount = ownUnits.size();
    int maxAttackUnits = 4;
    int maxCaptureUnits = 0;
    bool previusUnitWasCaptureUnit = false; //flag to indicate if the previous unit was a capture unit
    if (hasMedicUnit())
    {
        maxCaptureUnits = ownCount - maxAttackUnits - 1;
    }
    else
    {
        maxCaptureUnits = ownCount - maxAttackUnits;
    }
    if (maxCaptureUnits < 2)
    {
        maxCaptureUnits = 2;
    }      
    int attackUnits = 0;
    //count the number of units that are in Attack state
    for (auto& unit : ownUnits)
    {
        if (unit->getAiState()==ATTACK)
        {
            attackUnits++;
        }
    }
    int captureUnits = 0;
    //count the number of units that are in Capture state
    for (auto& unit : ownUnits)
    {
        if (unit->getAiState()==CAPTURE)
        {
            captureUnits++;
        }
    }
    needMoreUnits = 0;

    for (auto& unit : ownUnits)
    {
        if (unit->getType() == UnitType::medic)
        {
            unit->setAiState(NONE);
            previusUnitWasCaptureUnit = false;
        }
        else if (unit->getCurrentState() < 30)
        {
            unit->setAiState(RETREAT);
            previusUnitWasCaptureUnit = false;
        }
        else if (unitHoldsObjective(unit))
        {
            unit->setAiState(DEFEND);
            previusUnitWasCaptureUnit = false;
        }
        else if (!objectives.empty() && captureUnits < maxCaptureUnits && unit->getAiState()==NONE && !previusUnitWasCaptureUnit)
        {
            unit->setAiState(CAPTURE);
            captureUnits++;
            previusUnitWasCaptureUnit = true;
        }
        else if (!enemyUnits.empty() && attackUnits < maxAttackUnits && unit->getAiState()==NONE)
        {
            unit->setAiState(ATTACK);
            attackUnits++;
            previusUnitWasCaptureUnit = false;
        }
        /*else
        {
            unit->setAiState(NONE);
        }*/
    }
    needMoreUnits=maxAttackUnits-attackUnits;
}//end of determineState

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
                Unit* enemy = enemyUnits.front();
                Hex enemyHex = hexmap->getHex(enemy->getRow(), enemy->getCol());
                if (hexmap->distance(unit->getRow(), unit->getCol(), enemyHex.getRow(), enemyHex.getCol()) <= unit->getAttackRange())
                {
                    engageCombat(unit, enemy);
                }
                else
                {
                    moveToTargetsNeighbour(unit, enemyHex);
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
}// end of performActions

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
} //end of moveToTargetsNeighbour

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
} //end of moveToTarget

void AIManager::retreatToSafety(Unit* unit)
{
    if (hasMedicUnit())
    {
        //find medic unit
        for (auto& medic : ownUnits)
        {
            if (medic->getType() == UnitType::medic)
            {
                Hex medicHex = hexmap->getHex(medic->getRow(), medic->getCol());
                Hex unitHex = hexmap->getHex(unit->getRow(), unit->getCol());
                if (hexmap->distance(medicHex.getRow(), medicHex.getCol(), unitHex.getRow(), unitHex.getCol()) <= medic->getAttackRange())
                {
                    //heal unit
                    unit->setCurrentState(unit->getCurrentState() + 50);
                    medic->setActed();
                    break;
                }
                else
                {
                    moveToTargetsNeighbour(unit, medicHex);
                }
            }
        }
    }
    else
    {
        //no medic unit available, retreat to military base
        for (auto& base : *units)
        {
            if (base.getType() == UnitType::militarybase && base.getCountry() == currentPlayer)
            {
                Hex baseHex = hexmap->getHex(base.getRow(), base.getCol());
                Hex unitHex = hexmap->getHex(unit->getRow(), unit->getCol());
                if (hexmap->distance(baseHex.getRow(), baseHex.getCol(), unitHex.getRow(), unitHex.getCol()) <= unit->getAttackRange())
                {
                    moveToTargetsNeighbour(unit, baseHex);
                }
            }
        }
    }
}//end of retreatToSafety

void AIManager::captureObjective(Unit* unit, const Hex& objectiveHex)
//if target hex is empty, move to target hex
//if target hex is not empty, move to neighbour hex
{
    if (isEmptyField(objectiveHex.getRow(), objectiveHex.getCol()))
    {
        moveToTarget(unit, objectiveHex);
    }
    else
    {
        //find the unit on the objective hex
        Unit* target = nullptr;
        for (auto& unit : *units)
        {
            if (unit.getRow() == objectiveHex.getRow() && unit.getCol() == objectiveHex.getCol())
            {
                target = &unit;
                break;
            }
        }
        //if target is own unit set aiState to NONE
        if (target->getCountry() == currentPlayer)
        {
            unit->setAiState(NONE);
        }
        else if (hexmap->distance(unit->getRow(), unit->getCol(), objectiveHex.getRow(), objectiveHex.getCol()) <= unit->getAttackRange())
                {
                    engageCombat(unit, target);
                }
                else
                {
                    moveToTargetsNeighbour(unit, objectiveHex);
                }
    }
}//end of captureObjective

void AIManager::engageCombat(Unit* attacker, Unit* defender)
{
     if (mainWindow) {
        mainWindow->startCombat(*attacker, *defender);
        mainWindow->isAnybodyDead();
    }
}

void AIManager::buyUnits()
{
    // Medic hat oberste Priorität
    if (!hasMedicUnit() 
        && mainWindow->getPlayerBalances(currentPlayer) >= UnitType::getPrice(UnitType::medic))
    {
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
                        mainWindow->setPlayerBalances(currentPlayer, mainWindow->getPlayerBalances(currentPlayer) - UnitType::getPrice(UnitType::medic));
                        return; // Medic wurde gekauft, keine weiteren Aktionen notwendig
                    }
                }
            }
        }
    }

    // Kaufe andere Einheiten basierend auf verbleibenden Ressourcen
    std::vector<UnitType::Type> unitPriority = {
        UnitType::tank,         // Höchste Priorität nach Medic
        UnitType::fieldArtillery,
        UnitType::cavalry,
        UnitType::machineGun,
        UnitType::infantry      // Niedrigste Priorität
    };

    for (auto& unit : *units)
    {
        if (unit.getType() == UnitType::militarybase && unit.getCountry() == currentPlayer)
        {
            Hex baseHex = hexmap->getHex(unit.getRow(), unit.getCol());
            std::vector<Hex> neighbours = hexmap->getNeighborsSameTerritory(baseHex, baseHex.getTerritory());

            for (auto& unitType : unitPriority)
            {
                if (mainWindow->getPlayerBalances(currentPlayer) >= UnitType::getPrice(unitType))
                {
                    for (auto& neighbour : neighbours)
                    {
                        if (isEmptyField(neighbour.getRow(), neighbour.getCol()))
                        {
                            Unit newUnit(unitType, neighbour.getRow(), neighbour.getCol(), currentPlayer);
                            units->push_back(newUnit);
                            mainWindow->setPlayerBalances(currentPlayer, mainWindow->getPlayerBalances(currentPlayer) - UnitType::getPrice(unitType));
                            break;
                        }
                    }
                }
            }
        }
    }

    // Prüfe, ob noch Einheiten gekauft werden müssen
    
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

//checks if the player has a medic unit
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

//function that checks if a hexfield can be reached by a unit
bool AIManager::isReachable(Hex& startHex, Hex& targetHex)
{
    return (hexmap->calculateMovementCost(startHex.getRow(),startHex.getCol(),targetHex.getRow(),targetHex.getCol(),0)!=-1);
}

//checks if the unit posseses an city or industry   
bool AIManager::unitHoldsObjective(Unit* unit)
{
    for (auto& objective : CitiesAndIndustries)
    {
        if (unit->getRow() == objective->getRow() && unit->getCol() == objective->getCol())
        {
            return true;
        }
    }
    return false;   
}   

//checks if a unit can reach another unit
bool AIManager::isReachableUnit(Unit* unit, Unit* target, std::vector<Unit>* units)
{
    Hex startHex = hexmap->getHex(unit->getRow(), unit->getCol());
    Hex targetHex = hexmap->getHex(target->getRow(), target->getCol());
    return (hexmap->calculateMovementCost(startHex.getRow(),startHex.getCol(),targetHex.getRow(),targetHex.getCol(),0,units)!=-1);
}

