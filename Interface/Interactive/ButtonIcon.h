#pragma once

#include "../Virtual/Button.h"
#include <QPixmap>
#include <QGraphicsRectItem>
#include <QGraphicsItem>
#include <QGraphicsTextItem>
#include <QGraphicsPixmapItem>
#include <QVector>
#include <QColor>

enum class ButtonType {
    Like,
    Comment,
    AddBtm,
    Bookmarks,
    Expand

};

class ButtonIcon : public Button
{
public:
    ButtonIcon(ButtonType type, ColorScheme& scheme,
               QColor mainColor = nullptr, QGraphicsItem* paren = nullptr);

private:
    QColor mainCol;
    //ColorScheme scheme;
    QVector<QColor> colors;
    ButtonType btmType;
};


