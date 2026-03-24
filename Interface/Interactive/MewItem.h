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

class MewItem : public Button
{
public:
    MewItem(ColorScheme& scheme, QGraphicsItem* paren = nullptr);


private:
    QColor _textColor;
    QColor _titleColor;
    QColor _textGroupsColor;
    QColor _titleOptionalColor;
    QColor _blueActent;
    QPixmap _avatareGroup;


};

