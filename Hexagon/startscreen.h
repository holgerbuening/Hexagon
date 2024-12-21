#ifndef STARTSCREEN_H
#define STARTSCREEN_H

#include <QDialog>
#include <QGraphicsScene>
#include "ui_startscreen.h"

// Forward declaration
class MainWindow;


namespace Ui {
class StartScreen;
}

class StartScreen : public QDialog
{
    Q_OBJECT

public:
    explicit StartScreen(MainWindow *mainWindow, QWidget *parent = nullptr);
    ~StartScreen();
    void updateButtonStatus();

private slots:
    void on_pushButton_play_clicked();
    void on_pushButton_create_clicked();
    void on__load_clicked();
    void on_pushButton_end_clicked();
    void on_pushButton_save_clicked();
    void on_pushButtonEditMap_clicked();
    void on_pushButtonSaveMap_clicked();
    void on_pushButtonLoadMap_clicked();
    void on_pushButtonResumeGame_clicked();
    void on_pushButtonStopGame_clicked();
    void closeEvent(QCloseEvent *event);

private:
    Ui::StartScreenDialog *ui;
    QGraphicsScene *scene;
    MainWindow *mainWindow;
};

#endif // STARTSCREEN_H
