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
    Ui::HeadquarterDialog *ui;  // automatically generated UI-Object
    QStringListModel *unitModel; // Model for ListView
    std::unique_ptr<QGraphicsScene> scene; //scene for GraphicsView element
    std::unique_ptr<QGraphicsScene> backgroundScene;
    std::unique_ptr<QLabel> overlayLabel;
    QList<UnitType::Type> availableUnitTypes;
    int playerBalance;
    UnitType::Type selectedUnitType;

    void updateOkButtonState(int unitPrice);
};


#endif // HEADQUARTERDIALOG_H
