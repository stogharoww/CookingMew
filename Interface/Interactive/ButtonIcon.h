#pragma once

#include "../Virtual/Button.h"
#include <QPixmap>
#include <QGraphicsRectItem>
#include <QGraphicsItem>
#include <QGraphicsTextItem>
#include <QGraphicsPixmapItem>
#include <QVector>

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
    ButtonIcon(ButtonType btmType, ColorScheme& scheme,
               QColor mainColor = nullptr, QGraphicsItem* paren = nullptr);

private:

};


