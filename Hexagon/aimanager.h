#ifndef AIMANAGER_H
#define AIMANAGER_H

#include <vector>
#include "unit.h"
#include "hexmap.h"
#include "hex.h"   

class MainWindow; // Forward declaration of MainWindow

class AIManager
{
public:
    AIManager(MainWindow* mainWindow, HexMap* hexmap, std::vector<Unit>* units, const QString& currentPlayer, const QString& opponentPlayer);
    ~AIManager();
    void processTurn(); // Main function to handle AI logic

private:
    MainWindow* mainWindow; // Pointer to MainWindow
    HexMap* hexmap;
    std::vector<Unit>* units;
    QString currentPlayer;
    QString opponentPlayer;
    int needMoreUnits = 0; // Flag to indicate if the AI needs more units

    std::vector<Unit*> enemyUnits;
    std::vector<Unit*> ownUnits;
    std::vector<Unit*> ownFreeUnits;
    std::vector<Hex*> objectives;
    std::vector<Hex*> CitiesAndIndustries;

    void determineState(); // Determine AI state for each unit
    void performActions(); // Perform actions for each unit based on state
    void buyUnits(); // Buy units based on available resources

    void moveToTarget(Unit* unit, const Hex& targetHex);
    void moveToTargetsNeighbour(Unit* unit, const Hex& targetHex);
    void retreatToSafety(Unit* unit);
    void captureObjective(Unit* unit, const Hex& objectiveHex);
    void engageCombat(Unit* attacker, Unit* defender);
    void classifyUnits(); // Classify units into enemyUnits, ownUnits, and objectives
    bool unitHoldsObjective(Unit* unit); // Check if the unit holds an objective
    bool isEmptyField(int row, int col); // Check if the field is empty
    bool hasMedicUnit(); // Check if the player has a medic unit
};

#endif // AIMANAGER_H

