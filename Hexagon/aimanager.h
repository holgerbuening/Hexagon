#ifndef AIMANAGER_H
#define AIMANAGER_H

#include <vector>
#include "unit.h"
#include "hexmap.h"

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

    std::vector<Unit*> enemyUnits;
    std::vector<Unit*> ownUnits;
    std::vector<Unit*> objectives;

    void determineState(); // Determine AI state for each unit
    void performActions(); // Perform actions for each unit based on state

    void moveToTarget(Unit* unit, const Hex& targetHex);
    void retreatToSafety(Unit* unit);
    void captureObjective(Unit* unit, const Hex& objectiveHex);
    void engageCombat(Unit* attacker, Unit* defender);
    void classifyUnits(); // Classify units into enemyUnits, ownUnits, and objectives
};

#endif // AIMANAGER_H

