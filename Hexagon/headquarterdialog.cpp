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


#include "headquarterdialog.h"
#include "unittype.h"
#include <QGraphicsPixmapItem>
#include <QPushButton>

HeadquarterDialog::HeadquarterDialog(int playerBalance, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::HeadquarterDialog),
    unitModel(new QStringListModel(this)),
    scene(new QGraphicsScene(this)),
    playerBalance(playerBalance)
{
    ui->setupUi(this);
    ui->listView_units->setModel(unitModel); //link listview element with the Model
    ui->graphicsView->setScene(scene);//link scene with GraphicsView element
    ui->textBrowserBalance->setText(QString::number(playerBalance));
    ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
    connect(ui->listView_units, &QListView::clicked, this, &HeadquarterDialog::onUnitSelected);
}


HeadquarterDialog::~HeadquarterDialog()
{
    delete ui;
}

void HeadquarterDialog::populateUnitList(const QList<UnitType::Type> &unitTypes)
{
    QStringList unitStrings;
    availableUnitTypes=unitTypes;

    for(const UnitType::Type type : unitTypes)
    {
        QString unitString = QString("%1 - Price: %2")
                                 .arg(UnitType::getName(type))
                                 .arg(UnitType::getPrice(type));
        unitStrings.append(unitString);
    }
    unitModel->setStringList(unitStrings);
}

// Slot für die Auswahl in der ListView
void HeadquarterDialog::onUnitSelected(const QModelIndex &index)
{
    // Ermittle den ausgewählten UnitType basierend auf dem Index
    int selectedIndex = index.row();
    if (selectedIndex >= 0 && selectedIndex < availableUnitTypes.size()) {
        selectedUnitType = availableUnitTypes[selectedIndex];

        // Lade das Pixmap für die ausgewählte Einheit
        QPixmap unitPixmap = UnitType::getPixmap(selectedUnitType);

        // Lösche die alte Szene und füge das neue Bild hinzu
        scene->clear();
        scene->addPixmap(unitPixmap);

        // Skaliere das Pixmap in der GraphicsView (optional)
        ui->graphicsView->fitInView(scene->itemsBoundingRect(), Qt::KeepAspectRatio);

        //update OK Button depending on price and balance
        int unitPrice = UnitType::getPrice(selectedUnitType);
        updateOkButtonState(unitPrice);


    }
}

UnitType::Type HeadquarterDialog::getSelectedUnitType() const
{
    return selectedUnitType;
}

void HeadquarterDialog::updateOkButtonState(int unitPrice)
{
    if (unitPrice <= playerBalance) {
        // Wenn der Preis innerhalb des Budgets liegt, aktiviere den OK-Button
        ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(true);
    } else {
        // Andernfalls deaktiviere den OK-Button
        ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
    }
}
