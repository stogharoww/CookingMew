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
#include <QRectF>
#include <QPainter>
#include <QPen>

class MewItem : public Button
{
public:
    MewItem(ColorScheme& scheme, QRectF &globalRect, QGraphicsItem* paren = nullptr);
    void set_text(QString &text);
    void paint(QPainter* painter, const QStyleOptionGraphicsItem*, QWidget*) override;
    QRectF boundingRect() const override;



private:
    QColor _textColor;
    QColor _titleColor;
    QColor _textGroupsColor;
    QColor _titleOptionalColor;
    QColor _blueActent;
    QPixmap _avatareGroup;
    QGraphicsRectItem *mainRectItem;
    QRectF mainRect;


};

