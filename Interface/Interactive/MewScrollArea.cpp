#include "MewScrollArea.h"
#include "../Items/FollowLent.h"



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


void MewScrollArea::setContent(QGraphicsObject* item)
{
    _content = item;
    _content->setParentItem(this);
    _content->setPos(0, 0);

    // Пытаемся привести к FollowLent
    FollowLent* lent = qobject_cast<FollowLent*>(item);
    if (lent)
    {
        connect(lent, &FollowLent::openRecepie,
                this, [this](int id){
                    qDebug() << "[ScrollArea] got openRecepie, id =" << id;
                    emit openRecipe(id);
                });
    }

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

