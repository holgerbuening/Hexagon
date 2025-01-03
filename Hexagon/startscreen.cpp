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


#include "startscreen.h"
#include <QCloseEvent>
#include "mainwindow.h"
#include "stylemanager.h"


StartScreen::StartScreen(MainWindow *mainWindow, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::StartScreenDialog),
    scene(new QGraphicsScene(this)),
    mainWindow(mainWindow)
{
    ui->setupUi(this);

    // Set stylesheets for hover effect on buttons
    QString buttonStyle = StyleManager::buttonStyle();

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
