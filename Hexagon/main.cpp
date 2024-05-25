#include <QApplication>
#include "mainwindow.h" // Stelle sicher, dass du den richtigen Pfad zu deiner MainWindow-Klasse angibst

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    MainWindow w; // Erstelle eine Instanz deines Hauptfensters
    w.showMaximized(); // Zeige das Hauptfenster an

    return app.exec(); // Starte die Event-Schleife
}




