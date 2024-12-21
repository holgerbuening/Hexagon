#include "startscreen.h"
#include <QCloseEvent>
#include "mainwindow.h"


StartScreen::StartScreen(MainWindow *mainWindow, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::StartScreenDialog),
    scene(new QGraphicsScene(this)),
    mainWindow(mainWindow)
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
            QPushButton:disabled {
                background-color: rgb(200, 200, 200); /* Gray background for disabled state */
                color: rgb(150, 150, 150); /* Light gray text color for disabled state */
                border: 2px solid rgb(190, 190, 190); /* Light gray border for disabled state */
            }
        )";

    // Apply the style to all buttons in the StartScreen
    ui->pushButton_play->setStyleSheet(buttonStyle);
    ui->pushButton_end->setStyleSheet(buttonStyle);
    ui->pushButton_create->setStyleSheet(buttonStyle);
    ui->pushButton_save->setStyleSheet(buttonStyle);
    ui->_load->setStyleSheet(buttonStyle);
    ui->pushButtonEditMap->setStyleSheet(buttonStyle);
    ui->pushButtonSaveMap->setStyleSheet(buttonStyle);
    ui->pushButtonLoadMap->setStyleSheet(buttonStyle);
    ui->pushButtonResumeGame->setStyleSheet(buttonStyle);
    ui->pushButtonStopGame->setStyleSheet(buttonStyle);

    ui->pushButton_play->setEnabled(true); // Start new game is always enabled
    ui->_load->setEnabled(true); // Load game is always enabled
    ui->pushButton_end->setEnabled(true); // Exit is always enabled
    ui->pushButtonLoadMap->setEnabled(true); // Load map is always enabled
    ui->pushButtonSaveMap->setEnabled(true); // Save map is always enabled
    ui->pushButton_create->setEnabled(true); // Create map is always enabled
    
    updateButtonStatus();


     // Set fixed size for the dialog to disable resizing
    setFixedSize(this->width(), this->height());

    // Set window flags to disable the close button and keep the dialog on top
    setWindowFlags(Qt::Dialog | Qt::FramelessWindowHint);

    // Disable scrollbars for the QGraphicsView
    ui->graphicsView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->graphicsView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->graphicsView->setDragMode(QGraphicsView::NoDrag);

  
  
    // set background for GraphicsView
    QPixmap background(":/Images/start_screen.png");
    QPixmap scaledBackground = background.scaled
    (
        ui->graphicsView->width(),
        ui->graphicsView->height(),
        Qt::KeepAspectRatio,
        Qt::SmoothTransformation
    );
    scene->addPixmap(scaledBackground);
    ui->graphicsView->setScene(scene);
    
}

StartScreen::~StartScreen()
{
    delete ui;
    delete scene;
}

void StartScreen::updateButtonStatus()
{
    if (mainWindow->getGameModeStatus())
    {
        ui->pushButtonEditMap->setEnabled(false);
        ui->pushButtonResumeGame->setEnabled(true);
        ui->pushButtonStopGame->setEnabled(true);
        ui->pushButton_save->setEnabled(true);
    }
    else
    {
        ui->pushButtonEditMap->setEnabled(true);
        ui->pushButtonResumeGame->setEnabled(false);
        ui->pushButtonStopGame->setEnabled(false);
        ui->pushButton_save->setEnabled(false);
    }
}

// Close the dialog and return "accept"
void StartScreen::on_pushButton_play_clicked()
{
    //do all steps needed to start a new game
    mainWindow->startGameMode();
    this->accept();  
}

void StartScreen::on_pushButtonResumeGame_clicked()
{
    this->accept();
}

void StartScreen::on_pushButton_create_clicked()
{
    if (mainWindow)
    {
        mainWindow->createNewMap();
    }
    updateButtonStatus();
    //this->accept();
}

void StartScreen::on_pushButtonSaveMap_clicked()
{
    if (mainWindow)
    {
        mainWindow->saveAMap();
    }
    updateButtonStatus();
    //this->accept();
}

void StartScreen::on_pushButtonLoadMap_clicked()
{
    if (mainWindow)
    {
        mainWindow->loadAMap();
    }
    updateButtonStatus();
    //this->accept();
}    

void StartScreen::on__load_clicked()
{
    if (mainWindow)
    {
        mainWindow->loadAGame();
    }
    this->accept();
}


void StartScreen::on_pushButton_save_clicked()
{
    if (mainWindow)
    {
        mainWindow->saveAGame();
    }
    this->accept();
}

void StartScreen::on_pushButton_end_clicked()
{
    this->reject();
}

void StartScreen::on_pushButtonEditMap_clicked()
{
    if (mainWindow)
    {
        mainWindow->startEditMapMode();
    }
    this->accept();
}

void StartScreen::on_pushButtonStopGame_clicked()
{
    if (mainWindow)
    {
        mainWindow->stopGameMode();
    }
    updateButtonStatus();
    //this->reject();
}

// Override the close event to prevent closing the dialog via the "X" button
void StartScreen::closeEvent(QCloseEvent *event)
{
    // Ignore the close event
    event->ignore();
}
