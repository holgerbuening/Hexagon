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
    std::unique_ptr<Ui::CombatDialog> ui;
    Unit& attacker;
    Unit& defender;
    HexMap *hexmap;
    QPixmap* flagAttacker;
    QPixmap* flagDefender;
    std::unique_ptr<QGraphicsScene> sceneFlagAttacker;
    std::unique_ptr<QGraphicsScene> sceneFlagDefender;
    std::unique_ptr<QGraphicsPixmapItem> itemFlagAttacker;
    std::unique_ptr<QGraphicsPixmapItem> itemFlagDefender;
    std::unique_ptr<QPixmap> pixmapAttacker;
    std::unique_ptr<QPixmap> pixmapDefender;
    std::unique_ptr<QGraphicsScene> sceneUnitAttacker;
    std::unique_ptr<QGraphicsScene> sceneUnitDefender;
    std::unique_ptr<QGraphicsPixmapItem> itemUnitAttacker;
    std::unique_ptr<QGraphicsPixmapItem> itemUnitDefender;
    int result;
    int damageDefender;
    int damageAttacker;

    void calculateCombat();
};

#endif // COMBATDIALOG_H
