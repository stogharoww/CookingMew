#include "SquareTextBtm.h"
#include <QPainter>




SquareTextBtm::SquareTextBtm(ColorScheme &scheme, QGraphicsItem *paren)
    : Button(scheme, paren),
    scheme(scheme)
{
    QRectF paintingRect(0, 0, 100, 50);
    setBoundingRect(paintingRect);
    update();
}

void SquareTextBtm::setSize(qreal w, qreal h)
{
    QRectF newRect(0, 0, w, h);
    setBoundingRect(newRect);
    update();
}

void SquareTextBtm::setText(QString &text)
{
    _text = text;
    update();
}

void SquareTextBtm::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
    QPen pen(scheme.borderGet(), 1);
    painter->setPen(pen);
    painter->setBrush(currentBrush);
    painter->drawRect(boundingRect());

    painter->setFont(font);
    painter->setPen(QPen(scheme.secondColorGet(), 2));
    painter->drawText(boundingRect(), Qt::AlignCenter, _text);
}

void SquareTextBtm::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    currentBrush = QBrush(scheme.backgroundGet());
    update();
}

void SquareTextBtm::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    currentBrush = QBrush(scheme.backgroundGet().lighter(150));
    update();
    emit clicked();
}

void SquareTextBtm::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    currentBrush = QBrush(scheme.backgroundGet().lighter(150));
    update();
}

void SquareTextBtm::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    currentBrush = QBrush(scheme.backgroundGet());
    update();
}


