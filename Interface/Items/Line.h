#pragma once

#include <QGraphicsItem>
#include "../ColorScheme.h"
#include <QVector>
#include <QPointF>
#include <QRectF>

class Line : public QGraphicsItem
{
public:
    Line(ColorScheme scheme);

    void addLine(QPointF from, QPointF to);
    void addLine(QPointF from, QPointF to, QColor color);
    void changeColor(QColor color, int w = 1);
    void moveTo(QPointF from, QPointF to);

    QRectF boundingRect() const override;
    void paint(QPainter* painter,
               const QStyleOptionGraphicsItem*,
               QWidget*) override;

private:
    void updateBoundingRect();

    ColorScheme _scheme;
    QColor _mainColor;

    QVector<QPair<QPointF,QPointF>> _lines;
    QVector<QColor> _colors;
    int _w = 1;
    QRectF _bounds;
};
