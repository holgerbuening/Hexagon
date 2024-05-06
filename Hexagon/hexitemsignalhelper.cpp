#include "hexitemsignalhelper.h"
#include "hexitem.h" // Stelle sicher, dass du HexItem hier inkludierst.

HexItemSignalHelper::HexItemSignalHelper(QObject *parent)
    : QObject(parent) {
    // Konstruktor-Implementierung, normalerweise brauchst du hier nichts Spezielles zu tun.
}

// Das Signal selbst benötigt keine Implementierung im CPP, da es von Q_OBJECT Makro und dem Signal-Slot-System von Qt behandelt wird.
