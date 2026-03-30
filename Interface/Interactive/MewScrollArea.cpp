#include "MewScrollArea.h"



MewScrollArea::MewScrollArea(QRectF viewportRect, QGraphicsItem* parent)
    : QGraphicsObject(parent),
    _viewport(viewportRect)
{
    setFlag(QGraphicsItem::ItemClipsChildrenToShape, true);
    setAcceptedMouseButtons(Qt::AllButtons);
    setAcceptHoverEvents(true);
}

QRectF MewScrollArea::boundingRect() const
{
    return _viewport;
}

void MewScrollArea::paint(QPainter* painter,
                          const QStyleOptionGraphicsItem*, QWidget*){}


void MewScrollArea::setContent(QGraphicsItem* item)
{
    _content = item;
    _content->setParentItem(this);
    _content->setPos(0, 0);
}

void MewScrollArea::wheelEvent(QGraphicsSceneWheelEvent* event)
{
    if (!_content)
        return;

    _scrollY += event->delta() / 2.0;

    qreal contentHeight = _content->boundingRect().height();
    qreal viewportHeight = _viewport.height();

    // ограничение сверху
    if (_scrollY > 0)
        _scrollY = 0;

    // ограничение снизу
    qreal maxScroll = viewportHeight - contentHeight;
    if (_scrollY < maxScroll)
        _scrollY = maxScroll;

    _content->setPos(0, _scrollY);
}

