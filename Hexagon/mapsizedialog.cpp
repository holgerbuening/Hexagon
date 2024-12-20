#include "mapsizedialog.h"
/*#include "ui_mapSizeDialog.h"
#include <QDialog>*/

MapSizeDialog::MapSizeDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::mapSizeDialog)
{
    ui->setupUi(this);

    // Set stylesheets for hover effect on buttons
    QString buttonStyle = R"(
        QPushButton {
            background-color:rgb(158, 175, 76); /* Normal background color */
            color: white; /* Normal text color */
            border: 2px solidrgb(170, 176, 68);
            border-radius: 5px;
            padding: 5px;
        }
        QPushButton:hover {
            background-color:rgb(170, 181, 72); /* Highlighted background color on hover */
            border: 2px solidrgb(170, 176, 68);
        }
        QPushButton:pressed {
            background-color:rgb(128, 154, 43); /* Background color when pressed */
        }
    )";
    ui->pushButtonAccept->setStyleSheet(buttonStyle);
    ui->pushButtonCancel->setStyleSheet(buttonStyle);

    // Optional: Set fixed size if resizing is undesired
    setFixedSize(this->size());
    setWindowModality(Qt::ApplicationModal); // Ensures full application modality
    
    // Set window flags to disable the close button and keep the dialog on top
    setWindowFlags(Qt::Dialog | Qt::FramelessWindowHint);

    // Load the background image into the QGraphicsView
    QGraphicsScene *scene = new QGraphicsScene(this);
    QPixmap background(":/Images/military_map.png");
    scene->addPixmap(background);
    ui->graphicsViewMap->setScene(scene);
    ui->graphicsViewMap->setRenderHint(QPainter::Antialiasing);

    // Set default values for spin boxes
    ui->spinBoxWidth->setRange(10, 50);  // Minimum and maximum width
    ui->spinBoxHeight->setRange(10, 30); // Minimum and maximum height
    ui->spinBoxWidth->setValue(20);     // Default width
    ui->spinBoxHeight->setValue(12);    // Default height

     // Connect signals to slots for buttons
    connect(ui->pushButtonAccept, &QPushButton::clicked, this, &QDialog::accept);
    connect(ui->pushButtonCancel, &QPushButton::clicked, this, &QDialog::reject);

}

MapSizeDialog::~MapSizeDialog()
{
    delete ui;
}

int MapSizeDialog::getMapWidth() const
{
    return ui->spinBoxWidth->value();
}

int MapSizeDialog::getMapHeight() const
{
    return ui->spinBoxHeight->value();
}
