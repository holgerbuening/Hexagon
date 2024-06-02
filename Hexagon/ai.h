#ifndef AI_H
#define AI_H
#include "unit.h"
#include "hexmap.h"

enum AIState {
    ATTACK,
    DEFEND,
    CAPTURE,
    RETREAT
};

class AI {
public:
    void decide(Unit& unit, HexMap& map, std::vector<Unit>& enemyUnits, std::vector<Hex>& objectives, std::vector<Unit> *allUnits);

private:
    AIState determineState(Unit& unit, HexMap& map, std::vector<Unit>& enemyUnits, std::vector<Hex>& objectives);
    void performAction(Unit& unit, AIState state, HexMap& map, std::vector<Unit>& enemyUnits, std::vector<Hex>& objectives, std::vector<Unit> *allUnits);
};

#endif // AI_H
