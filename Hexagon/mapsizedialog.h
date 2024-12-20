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
};

#endif // MAPSIZEDIALOG_H
