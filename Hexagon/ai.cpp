#include <algorithm>
#include "ai.h"
#include "mainwindow.h"

void AI::decide(Unit& unit, HexMap& map, std::vector<Unit>& enemyUnits, std::vector<Hex>& objectives, std::vector<Unit> *allUnits)
{
    AIState state = determineState(unit, map, enemyUnits, objectives);
    performAction(unit, state, map, enemyUnits, objectives, allUnits);
}

AIState AI::determineState(Unit& unit, HexMap& map, std::vector<Unit>& enemyUnits, std::vector<Hex>& objectives) {
    // Beispielhafte Zustandsbestimmung
    if (unit.getCurrentState() < 30) {
        return RETREAT;
    }

    if (!enemyUnits.empty()) {
        return ATTACK;
    }

    if (!objectives.empty()) {
        return CAPTURE;
    }

    return DEFEND;
}

void AI::performAction(Unit& unit, AIState state, HexMap& map, std::vector<Unit>& enemyUnits, std::vector<Hex>& objectives, std::vector<Unit> *allUnits) {
    switch (state) {
        case ATTACK:
            // Einfachster Gegner in Reichweite suchen und angreifen
            if (!enemyUnits.empty())
            {
                Hex start = map.getHex(unit.getRow(),unit.getCol());
                int territory = start.getTerritory();
                Hex objective = map.getHex(objectives[0].getRow(),objectives[0].getCol());
                Hex target = map.getClosestNeighbourSameTerritoryNoUnits(start,objective,territory,allUnits);
                int attackRow = target.getRow();
                int attackCol = target.getCol();
                std::vector<Node> wayToTarget = map.AStar(map.getHex(unit.getRow(),unit.getCol()),map.getHex(attackRow,attackCol),unit.getTerritory(), allUnits);
                if (!wayToTarget.empty())
                {
                    Node targetNextMove = map.getReachableNode(wayToTarget,unit.getRemainingMovementPoints());
                    int distance=map.calculateMovementCost(unit.getRow(),unit.getCol(),targetNextMove.row,targetNextMove.col,territory,allUnits);
                    unit.moveTo(targetNextMove.row,targetNextMove.col,distance);
                    if (unit.getRemainingMovementPoints()<=0)
                    {
                        unit.setActed();
                    }
                    else if (map.calculateMovementCost(targetNextMove.row,targetNextMove.col,objective.getRow(),objective.getCol(),territory,allUnits)<=unit.getAttackRange())
                    {
                        //MainWindow::startCombat(unit,objective);
                    }
                }

            }
            break;
        case DEFEND:
            // Verteidige Position oder bewege dich zu einer defensiven Position
            break;
        case CAPTURE:
            // Bewege die Einheit zum nächstgelegenen Ziel
            if (!objectives.empty()) {
                const Hex& objective = objectives[0]; // Beispiel: Wähle das erste Ziel
                int captureRow = objective.getRow();
                int captureCol = objective.getCol();
                // Bewegung zum Ziel
                // Hier A*-Algorithmus zur Bewegung verwenden
            }
            break;
        case RETREAT:
            // Bewege die Einheit zur nächsten sicheren Position oder Basis
            break;
    }
}
