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
    explicit CombatDialog(Unit& attacker, Unit& defender, HexMap* hexmap, QWidget *parent = nullptr);
    ~CombatDialog();

    int getResult() const;

private slots:
    void on_okButton_clicked();

private:
    Ui::CombatDialog *ui;
    Unit& attacker;
    Unit& defender;
    HexMap *hexmap;

    int result;

    void calculateCombat();
};

#endif // COMBATDIALOG_H
