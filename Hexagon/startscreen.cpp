#include "startscreen.h"


StartScreen::StartScreen(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::StartScreenDialog),
    scene(new QGraphicsScene(this))
{
    ui->setupUi(this);

    // Setze ein Hintergrundbild für die GraphicsView
    QPixmap background(":/Images/start_screen.png");
    scene->addPixmap(background);
    ui->graphicsView->setScene(scene);
}

StartScreen::~StartScreen()
{
    delete ui;
    delete scene;
}

// Schließt das Fenster beim Klick auf "PushButton_play"
void StartScreen::on_pushButton_play_clicked()
{
    this->accept();  // Beendet den Dialog und gibt "accept" zurück
}
