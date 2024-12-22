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





#include <QApplication>
#include "mainwindow.h" // Stelle sicher, dass du den richtigen Pfad zu deiner MainWindow-Klasse angibst

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    MainWindow w; // Erstelle eine Instanz deines Hauptfensters
    w.showMaximized(); // Zeige das Hauptfenster an

    return app.exec(); // Starte die Event-Schleife
}




