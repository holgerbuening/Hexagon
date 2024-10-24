#ifndef HEADQUARTERDIALOG_H
#define HEADQUARTERDIALOG_H


#include <QDialog>
#include "ui_headquarter.h"
#include <QStringListModel>
#include <QListView>
#include <QGraphicsScene>
#include "unittype.h"

class HeadquarterDialog : public QDialog
{
    Q_OBJECT

public:
    explicit HeadquarterDialog(int playerBalance, QWidget *parent = nullptr);
    ~HeadquarterDialog();
    void populateUnitList(const QList<UnitType::Type> &unitTypes);
    UnitType::Type getSelectedUnitType() const;

private slots:
    void onUnitSelected(const QModelIndex &index);

private:
    Ui::Dialog *ui;  // automatically generated UI-Object
    QStringListModel *unitModel; // Model for ListView
    QGraphicsScene *scene; //scene for GraphicsView element
    QList<UnitType::Type> availableUnitTypes;
    int playerBalance;
    UnitType::Type selectedUnitType;

    void updateOkButtonState(int unitPrice);
};


#endif // HEADQUARTERDIALOG_H
