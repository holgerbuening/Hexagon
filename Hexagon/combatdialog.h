#ifndef COMBATDIALOG_H
#define COMBATDIALOG_H

#include <QDialog>
#include "unit.h"
#include "hexmap.h"

namespace Ui {
class CombatDialog;
}

class CombatDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CombatDialog(Unit& attacker, Unit& defender, HexMap* hexmap, QPixmap *flagAttacker, QPixmap *flagDefender, QWidget *parent = nullptr);
    ~CombatDialog();

    int getResult() const;
    int getDamageAttacker() const;
    int getDamageDefener() const;

private:
    Ui::CombatDialog *ui;
    Unit& attacker;
    Unit& defender;
    HexMap *hexmap;
    QPixmap *flagAttacker;
    QPixmap *flagDefender;
    QGraphicsScene *sceneFlagAttacker;
    QGraphicsScene *sceneFlagDefender;
    QGraphicsPixmapItem *itemFlagAttacker;
    QGraphicsPixmapItem *itemFlagDefender;
    QPixmap *pixmapAttacker;
    QPixmap *pixmapDefender;
    QGraphicsScene *sceneUnitAttacker;
    QGraphicsScene *sceneUnitDefender;
    QGraphicsPixmapItem *itemUnitAttacker;
    QGraphicsPixmapItem *itemUnitDefender;
    int result;
    int damageDefender;
    int damageAttacker;

    void calculateCombat();
};

#endif // COMBATDIALOG_H
