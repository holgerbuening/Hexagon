#include "combatdialog.h"
#include "ui_combatdialog.h"
#include "fieldtype.h"
#include "hex.h"
#include <cstdlib>

CombatDialog::CombatDialog(Unit& attacker, Unit& defender, HexMap *hexmap, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CombatDialog),
    attacker(attacker),
    defender(defender),
    hexmap(hexmap),
    result(0)
{
    ui->setupUi(this);
    calculateCombat();
}

CombatDialog::~CombatDialog()
{
    delete ui;
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
    result = attackPower - defensePower;
    if (result<=0)
    {
        result=0;
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
    ui->lcdNumberCombatResult->display(result);
}

int CombatDialog::getResult() const
{
    return result;
}

void CombatDialog::on_okButton_clicked()
{
    accept();
}
