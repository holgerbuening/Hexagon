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


#include "combatdialog.h"
#include "ui_combatdialog.h"
#include "fieldtype.h"
#include "hex.h"
#include <cstdlib>
#include "unittype.h"

CombatDialog::CombatDialog(Unit& attacker, Unit& defender, HexMap *hexmap, QPixmap *flagAttacker, QPixmap *flagDefender,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CombatDialog),
    attacker(attacker),
    defender(defender),
    hexmap(hexmap),
    flagAttacker(flagAttacker),
    flagDefender(flagDefender),
    sceneFlagAttacker(new QGraphicsScene(this)),
    sceneFlagDefender(new QGraphicsScene(this)),
    itemFlagAttacker(new QGraphicsPixmapItem(*flagAttacker)),
    itemFlagDefender(new QGraphicsPixmapItem(*flagDefender)),
    result(0),
    damageDefender(0),
    damageAttacker(0)
{
    ui->setupUi(this);
    //itemFlagAttacker = new QGraphicsPixmapItem(*flagAttacker);
    //itemFlagDefender = new QGraphicsPixmapItem(*flagDefender);
    sceneUnitAttacker = new QGraphicsScene(this);
    sceneUnitDefender = new QGraphicsScene(this);
    sceneFlagAttacker->addItem(itemFlagAttacker);
    sceneFlagDefender->addItem(itemFlagDefender);
    ui->graphicsViewflagAttacker->setScene(sceneFlagAttacker);
    ui->graphicsViewflagDefender->setScene(sceneFlagDefender);
    pixmapAttacker = new QPixmap(UnitType::getPixmap(attacker.getType()));
    pixmapDefender = new QPixmap(UnitType::getPixmap(defender.getType()));
    itemUnitAttacker = new QGraphicsPixmapItem(*pixmapAttacker);
    itemUnitDefender = new QGraphicsPixmapItem(*pixmapDefender);
    sceneUnitAttacker->addItem(itemUnitAttacker);
    sceneUnitDefender->addItem(itemUnitDefender);
    ui->graphicsViewUnitAttacker->setScene(sceneUnitAttacker);
    ui->graphicsViewUnitDefender->setScene(sceneUnitDefender);
    //ui->graphicsViewUnitAttacker->fitInView(itemUnitAttacker, Qt::KeepAspectRatio);
    ui->graphicsViewUnitAttacker->scale(0.22,0.22);
    ui->graphicsViewUnitAttacker->setRenderHint(QPainter::Antialiasing);
    ui->graphicsViewUnitAttacker->show();
    ui->graphicsViewUnitDefender->scale(0.22,0.22);
    ui->graphicsViewUnitDefender->setRenderHint(QPainter::Antialiasing);
    ui->graphicsViewUnitDefender->show();
    calculateCombat();



}

CombatDialog::~CombatDialog()
{
    if (ui!=nullptr) {delete ui;ui=nullptr;}
    if (sceneFlagAttacker!=nullptr) {delete sceneFlagAttacker; sceneFlagAttacker=nullptr;}
    if (sceneFlagDefender!=nullptr) {delete sceneFlagDefender;sceneFlagDefender=nullptr;}
    if (pixmapAttacker!=nullptr) {delete pixmapAttacker;pixmapAttacker=nullptr;}
    if (pixmapDefender!=nullptr) {delete pixmapDefender;pixmapDefender=nullptr;}
    if (sceneUnitAttacker!=nullptr) {delete sceneUnitAttacker;sceneUnitAttacker=nullptr;}
    if (sceneUnitDefender!=nullptr) {delete sceneUnitDefender;sceneUnitDefender=nullptr;}
    /*if (itemFlagAttacker!=nullptr) {delete itemFlagAttacker;itemFlagAttacker=nullptr;}
    if (itemFlagDefender!=nullptr) {delete itemFlagDefender;itemFlagDefender=nullptr;}
    if (itemUnitAttacker!=nullptr) {delete itemUnitAttacker;itemUnitAttacker=nullptr;}
    if (itemUnitDefender!=nullptr) {delete itemUnitDefender;itemUnitDefender=nullptr;}*/
}

void CombatDialog::calculateCombat()
{
    srand(time(NULL));
    // Calculate base offense and defense values
    int attackBase = attacker.getOffense();
    attackBase+=attacker.getExperience()*10;
    int defenseBase = defender.getDefense();
    defenseBase+=defender.getExperience()*10;
    
    // Add terrain defense bonus
    FieldType::Type fieldtype;
    Hex hex;
    hex = hexmap->getHex(defender.getRow(),defender.getCol());
    fieldtype = hex.getFieldType();
    int fieldDefense = FieldType::getDefense(fieldtype);
    defenseBase+=fieldDefense;

    // Calculate the distance between attacker and defender
    int distance = HexMap::distance(attacker.getRow(), attacker.getCol(), defender.getRow(), defender.getCol());

    // Check if the attacker is within the defender's attack range
    bool isWithinDefenderRange = (distance <= defender.getAttackRange());

    // Add random variations to defense power
    int randomRangeDefender = defenseBase * 0.10;
    int minDefender = defenseBase - randomRangeDefender;
    int maxDefender = defenseBase + randomRangeDefender;
    int randomDefender = rand() %100;
    float defenseFactor = randomDefender/100.0f;
    int defensePower = minDefender+static_cast<int>(defenseFactor*(maxDefender-minDefender));

    // Add random variations to attack power
    int randomRangeAttacker = attackBase * 0.25;
    int minAttacker=attackBase-randomRangeAttacker;
    int maxAttacker=attackBase+randomRangeAttacker*2;
    int randomAttacker = rand() % 100;
    float attackFactor = randomAttacker/100.0f;
    int attackPower = minAttacker+ static_cast<int>(attackFactor*(maxAttacker-minAttacker));
    
    // Calculate the result of the combat
    int result = attackPower - defensePower;
    int randomDamage = rand()%5 +1;

    // Determine damage to the defender
    if (result<5)
    { damageDefender= randomDamage;}
    else
    { damageDefender=result+randomDamage;}
    
    // Determine damage to the attacker
    if (isWithinDefenderRange)
    {
        if (result<0)
        { damageAttacker=result*-1+randomDamage;}
        else
        { damageAttacker=randomDamage;}
    }
    else
    {
        damageAttacker=0;
    }



    // Ergebnisse anzeigen
    ui->lineEditAttackerOffense->setText(QString::number(attackBase));
    ui->lineEditAttackerMin->setText(QString::number(minAttacker));
    ui->lineEditAttackerMax->setText(QString::number(maxAttacker));
    ui->lineEditAttackerRandom->setText(QString::number(randomAttacker));
    ui->lineEditAttackerFinal->setText(QString::number(attackPower));
    ui->lineEditDefenderDefense->setText(QString::number(defenseBase));
    ui->lineEditDefenderMin->setText(QString::number(minDefender));
    ui->lineEditDefenderMax->setText(QString::number(maxDefender));
    ui->lineEditDefenderRandom->setText(QString::number(randomDefender));
    ui->lineEditDefenderFinal->setText(QString::number(defensePower));
    QPalette palette = ui->lcdNumberResultDefender->palette();
    palette.setColor(QPalette::WindowText, Qt::green);
    ui->lcdNumberResultDefender->setPalette(palette);
    ui->lcdNumberResultDefender->display(damageDefender);
    palette = ui->lcdNumberResultAttacker->palette();
    palette.setColor(QPalette::WindowText, Qt::red);
    ui->lcdNumberResultAttacker->setPalette(palette);
    ui->lcdNumberResultAttacker->display(damageAttacker);
}

int CombatDialog::getResult() const
{
    return result;
}

int CombatDialog::getDamageAttacker() const
{
    return damageAttacker;
}

int CombatDialog::getDamageDefener() const
{
    return damageDefender;
}

