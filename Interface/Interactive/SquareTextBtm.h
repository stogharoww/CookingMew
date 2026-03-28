#pragma once

#include "../Virtual/Button.h"
#include <QVector>
#include <QString>
#include <QFont>
#include <QColor>
#include <QGraphicsItem>
#include <QGraphicsRectItem>
#include <QGraphicsTextItem>
#include <QPixmap>
#include "../ColorScheme.h"

class SquareTextBtm : public Button
{
public:
    SquareTextBtm(ColorScheme& scheme, QGraphicsItem* paren = nullptr);


};


