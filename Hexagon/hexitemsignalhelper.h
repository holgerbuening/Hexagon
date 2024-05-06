#ifndef HEXITEMSIGNALHELPER_H
#define HEXITEMSIGNALHELPER_H

#include <QObject>

class HexItem;  // Forward-Deklaration

class HexItemSignalHelper : public QObject {
    Q_OBJECT
public:
    HexItemSignalHelper(QObject *parent = nullptr);
signals:
    void itemSelected(HexItem *item);
};

#endif // HEXITEMSIGNALHELPER_H
