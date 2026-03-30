#include "MewItem.h"



MewItem::MewItem(ColorScheme &scheme, QRectF &globalRect, QGraphicsItem *paren)
    : Button(scheme, paren)
{
    mainRect = QRectF(0, 0, globalRect.width(), 500);
    mainRectItem = new QGraphicsRectItem(mainRect);
    mainRectItem->setParentItem(this);
    mainRectItem->setPen(QPen(scheme.borderGet(), 1));
    //mainRectItem->setBrush(QBrush(scheme.borderGet()));
}

void MewItem::set_text(QString &text)
{

}

void MewItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{

}

QRectF MewItem::boundingRect() const
{
    return mainRect;
}
