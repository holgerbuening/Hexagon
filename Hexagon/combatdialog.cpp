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
    result(0)

{
    ui->setupUi(this);
    itemFlagAttacker = new QGraphicsPixmapItem(*flagAttacker);
    itemFlagDefender = new QGraphicsPixmapItem(*flagDefender);
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
    ui->graphicsViewUnitAttacker->fitInView(itemUnitAttacker, Qt::KeepAspectRatio);
    ui->graphicsViewUnitAttacker->setRenderHint(QPainter::Antialiasing);
    ui->graphicsViewUnitAttacker->show();
    ui->graphicsViewUnitDefender->fitInView(itemUnitDefender, Qt::KeepAspectRatio);
    ui->graphicsViewUnitDefender->setRenderHint(QPainter::Antialiasing);
    ui->graphicsViewUnitDefender->show();
    calculateCombat();



}

CombatDialog::~CombatDialog()
{
    if (ui!=nullptr) {delete ui;ui=nullptr;}
    //if (hexmap!=nullptr) {delete hexmap; hexmap=nullptr;}
    //if (flagAttacker!=nullptr) {delete flagAttacker; flagAttacker=nullptr;}
    //if (flagDefender!=nullptr){delete flagDefender; flagDefender=nullptr;}
    if (sceneFlagAttacker!=nullptr) {delete sceneFlagAttacker; sceneFlagAttacker=nullptr;}
    if (sceneFlagDefender!=nullptr) {delete sceneFlagDefender;sceneFlagDefender=nullptr;}
    //if (itemFlagAttacker!=nullptr) {delete itemFlagAttacker; itemFlagAttacker=nullptr;}
    //if (itemFlagDefender!=nullptr) {delete itemFlagDefender; itemFlagDefender=nullptr;}
    if (pixmapAttacker!=nullptr) {delete pixmapAttacker;pixmapAttacker=nullptr;}
    if (pixmapDefender!=nullptr) {delete pixmapDefender;pixmapDefender=nullptr;}
    if (sceneUnitAttacker!=nullptr) {delete sceneUnitAttacker;sceneUnitAttacker=nullptr;}
    if (sceneUnitDefender!=nullptr) {delete sceneUnitDefender;sceneUnitDefender=nullptr;}
    //if (itemUnitDefender!=nullptr) {delete itemUnitAttacker;itemUnitAttacker=nullptr;}
    //if (itemUnitDefender!=nullptr) {delete itemUnitDefender;itemUnitDefender=nullptr;}
}

void CombatDialog::calculateCombat()
{
    srand(time(NULL));
    int attackBase = attacker.getOffense();
    int defenseBase = defender.getDefense();
    FieldType::Type fieldtype;
    Hex hex;
    hex = hexmap->getHex(defender.getRow(),defender.getCol());
    fieldtype = hex.getFieldType();
    int fieldDefense = FieldType::getDefense(fieldtype);
    defenseBase+=fieldDefense;
    int randomRangeDefender = defenseBase * 0.10;
    int minDefender = defenseBase - randomRangeDefender;
    int maxDefender = defenseBase + randomRangeDefender;
    int randomDefender = rand() %100;
    float defenseFactor = randomDefender/100.0f;
    int defensePower = minDefender+static_cast<int>(defenseFactor*(maxDefender-minDefender));
    int randomRangeAttacker = attackBase * 0.25;
    int minAttacker=attackBase-randomRangeAttacker;
    int maxAttacker=attackBase+randomRangeAttacker*2;
    int randomAttacker = rand() % 100;
    float attackFactor = randomAttacker/100.0f;
    int attackPower = minAttacker+ static_cast<int>(attackFactor*(maxAttacker-minAttacker));
    int result = attackPower - defensePower;
    int randomDamage = rand()%5 +1;
    if (result<5)
    { damageDefender= randomDamage;}
    else
    { damageDefender=result+randomDamage;}
    if (result<0)
    { damageAttacker=result*-1+randomDamage;}
    else
    { damageAttacker=randomDamage;}



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

