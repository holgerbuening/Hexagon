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
    playerBalance(playerBalance),
    availableUnitTypes(),
    selectedUnitType()
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
    delete unitModel;
    delete scene;

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

// Slot for the signal clicked() from the ListView
void HeadquarterDialog::onUnitSelected(const QModelIndex &index)
{
    // Set the selected unit type
    int selectedIndex = index.row();
    if (selectedIndex >= 0 && selectedIndex < availableUnitTypes.size()) {
        selectedUnitType = availableUnitTypes[selectedIndex];

        // Load the pixmap for the selected unit type
        QPixmap unitPixmap = UnitType::getPixmap(selectedUnitType);

        // Clear the scene and add the new pixmap
        scene->clear();
        scene->addPixmap(unitPixmap);

        // scale the pixmap to fit into the GraphicsView
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
        // if the player has enough money, enable the OK-Button
        ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(true);
    } else {
        // else disable the OK-Button
        ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
    }
}
