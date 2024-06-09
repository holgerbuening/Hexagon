#ifndef HEX_H
#define HEX_H

#include <QPixmap>
#include "fieldtype.h"
#include <QDataStream>

class Hex {
public:
    int getRow() const;
    int getCol() const;
    FieldType::Type getFieldType() const;
    const QString getFieldTypeText() const;
    void setFieldType(FieldType::Type fieldType);
    int getMovementCost() const;
    void setMovementCost(int newMovementCost);
    void setMovementCost();
    int getDefense() const;
    int getTerritory();
    void setDefense (int newDefense);
    const QPixmap& getPixmap();
    Hex(int col=0, int row=0, FieldType::Type fieldType=FieldType::Woods);
    bool operator==(const Hex &other) const {
           return row == other.row && col == other.col;}
    bool operator<(const Hex &other) const {
           if (row == other.row) {
                return col < other.col;
                  }
                return row < other.row;}

    friend QDataStream& operator<<(QDataStream& out, const Hex& hex);
    friend QDataStream& operator>>(QDataStream& in, Hex& hex);

private:
    int col, row;
    FieldType::Type fieldType;
    int movementCost;
    int defense;
};

QDataStream& operator<<(QDataStream& out, const Hex& hex);
QDataStream& operator>>(QDataStream& in, Hex& hex);


struct HashHex {
    std::size_t operator()(const Hex &hex) const {
        return std::hash<int>()(hex.getRow()) ^ (std::hash<int>()(hex.getCol()) << 1);
    }
};


#endif // HEX_H
