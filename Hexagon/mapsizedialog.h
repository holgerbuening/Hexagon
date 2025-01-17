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


#ifndef MAPSIZEDIALOG_H
#define MAPSIZEDIALOG_H

#include <QDialog>
#include "ui_mapSizeDialog.h"

/*namespace Ui {
class MapSizeDialog;
}*/

class MapSizeDialog : public QDialog
{
    Q_OBJECT

public:
    explicit MapSizeDialog(QWidget *parent = nullptr);
    ~MapSizeDialog();

    // Getter for width and height
    int getMapWidth() const;
    int getMapHeight() const;

private:
    Ui::mapSizeDialog *ui;
    //QGraphicsScene *scene;
};

#endif // MAPSIZEDIALOG_H
