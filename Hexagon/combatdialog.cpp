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
#include "stylemanager.h"

CombatDialog::CombatDialog(Unit& attacker, Unit& defender, HexMap *hexmap, QPixmap *flagAttacker, QPixmap *flagDefender,QWidget *parent) :
    QDialog(parent),
    ui(std::make_unique<Ui::CombatDialog>()),
    attacker(attacker),
    defender(defender),
    hexmap(hexmap),
    flagAttacker(flagAttacker),
    flagDefender(flagDefender),
    sceneBackground(std::make_unique<QGraphicsScene>(this)),
    sceneFlagAttacker(std::make_unique<QGraphicsScene>(this)),
    sceneFlagDefender(std::make_unique<QGraphicsScene>(this)),
    itemFlagAttacker(std::make_unique<QGraphicsPixmapItem>(*flagAttacker)),
    itemFlagDefender(std::make_unique<QGraphicsPixmapItem>(*flagDefender)),
    result(0),
    damageDefender(0),
    damageAttacker(0),
    pixmapAttacker(std::make_unique<QPixmap>(UnitType::getPixmap(attacker.getType()))),
    pixmapDefender(std::make_unique<QPixmap>(UnitType::getPixmap(defender.getType()))),
    sceneUnitAttacker(std::make_unique<QGraphicsScene>(this)),
    sceneUnitDefender(std::make_unique<QGraphicsScene>(this)),
    itemUnitAttacker(std::make_unique<QGraphicsPixmapItem>(*pixmapAttacker)),
    itemUnitDefender(std::make_unique<QGraphicsPixmapItem>(*pixmapDefender))
{
    ui->setupUi(this);
    sceneFlagAttacker->addItem(itemFlagAttacker.get());
    sceneFlagDefender->addItem(itemFlagDefender.get());
    ui->graphicsViewflagAttacker->setScene(sceneFlagAttacker.get());
    ui->graphicsViewflagDefender->setScene(sceneFlagDefender.get());
    sceneUnitAttacker->addItem(itemUnitAttacker.get());
    sceneUnitDefender->addItem(itemUnitDefender.get());
    ui->graphicsViewUnitAttacker->setScene(sceneUnitAttacker.get());
    ui->graphicsViewUnitDefender->setScene(sceneUnitDefender.get());
    //ui->graphicsViewUnitAttacker->fitInView(itemUnitAttacker, Qt::KeepAspectRatio);
    ui->graphicsViewUnitAttacker->scale(0.22,0.22);
    ui->graphicsViewUnitAttacker->setRenderHint(QPainter::Antialiasing);
    ui->graphicsViewUnitAttacker->show();
    ui->graphicsViewUnitDefender->scale(0.22,0.22);
    ui->graphicsViewUnitDefender->setRenderHint(QPainter::Antialiasing);
    ui->graphicsViewUnitDefender->show();
    
     // Set fixed size for the dialog to disable resizing
    setFixedSize(this->width(), this->height());

    // Set window flags to disable the close button and keep the dialog on top
    setWindowFlags(Qt::Dialog | Qt::FramelessWindowHint);

    
    
    
    // Load the background image into the QGraphicsView
    QPixmap background(":/Images/combatbackground.png");
    sceneBackground->addPixmap(background);
    ui->graphicsViewBackground->setGeometry(this->rect());
    ui->graphicsViewBackground->setScene(sceneBackground.get());
    ui->graphicsViewBackground->setRenderHint(QPainter::Antialiasing);

    ui->lineEditAttackerFinal->setStyleSheet(StyleManager::transparentLineEditStyle());
    ui->lineEditDefenderFinal->setStyleSheet(StyleManager::transparentLineEditStyle());
    ui->lineEditAttackerMax->setStyleSheet(StyleManager::transparentLineEditStyle());
    ui->lineEditDefenderMax->setStyleSheet(StyleManager::transparentLineEditStyle());
    ui->lineEditAttackerMin->setStyleSheet(StyleManager::transparentLineEditStyle());
    ui->lineEditDefenderMin->setStyleSheet(StyleManager::transparentLineEditStyle());
    ui->lineEditAttackerOffense->setStyleSheet(StyleManager::transparentLineEditStyle());
    ui->lineEditDefenderDefense->setStyleSheet(StyleManager::transparentLineEditStyle());
    ui->lineEditAttackerRandom->setStyleSheet(StyleManager::transparentLineEditStyle());
    ui->lineEditDefenderRandom->setStyleSheet(StyleManager::transparentLineEditStyle());

    this->setStyleSheet(StyleManager::whiteLabelStyle());

    ui->graphicsViewUnitAttacker->setStyleSheet("background: transparent;");
    ui->graphicsViewUnitDefender->setStyleSheet("background: transparent;");
    ui->pushButtonOK->setStyleSheet(StyleManager::buttonStyle());
    connect(ui->pushButtonOK, &QPushButton::clicked, this, &QDialog::accept);

    calculateCombat();



}

CombatDialog::~CombatDialog()
{
    qDebug() << "CombatDialog destructor:";
    qDebug() << "ui is Smartpointer and will be deleted automatically";
    qDebug() << "sceneFlagAttacker is Smartpointer and will be deleted automatically";
    qDebug() << "sceneFlagDefender is Smartpointer and will be deleted automatically";
    qDebug() << "itemFlagAttacker is Smartpointer and will be deleted automatically";
    qDebug() << "itemFlagDefender is Smartpointer and will be deleted automatically";
    qDebug() << "pixmapAttacker is Smartpointer and will be deleted automatically";
    qDebug() << "pixmapDefender is Smartpointer and will be deleted automatically";
    qDebug() << "sceneUnitAttacker is Smartpointer and will be deleted automatically";
    qDebug() << "sceneUnitDefender is Smartpointer and will be deleted automatically";
    qDebug() << "itemUnitAttacker is Smartpointer and will be deleted automatically";
    qDebug() << "itemUnitDefender is Smartpointer and will be deleted automatically";
    if (hexmap!=nullptr) 
    {   
        qDebug() << "hexmap is not nullptr and will be deleted";
        //delete hexmap;
        hexmap=nullptr;
    }
    qDebug() << "CombatDialog destructor finished";
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

