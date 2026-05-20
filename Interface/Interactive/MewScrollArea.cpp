#include "MewScrollArea.h"
#include <QDebug>
#include "../Items/FollowLent.h"

MewScrollArea::MewScrollArea(QRectF viewportRect, QGraphicsItem* parent)
    : QGraphicsObject(parent),
    _viewport(viewportRect)
{
    setFlag(ItemClipsChildrenToShape, true);
    setAcceptedMouseButtons(Qt::AllButtons);
    setAcceptHoverEvents(true);
}

QRectF MewScrollArea::boundingRect() const
{
    return _viewport;
}

void MewScrollArea::paint(QPainter*, const QStyleOptionGraphicsItem*, QWidget*)
{
    // ScrollArea сама ничего не рисует — только клипает детей
}

void MewScrollArea::setContent(QGraphicsObject* item)
{
    _content = item;
    _content->setParentItem(this);
    _content->setPos(0, 0);

     _content->installSceneEventFilter(this);

    // если это лента — прокидываем сигнал
    if (auto* lent = qobject_cast<FollowLent*>(item))
    {
        connect(lent, &FollowLent::openRecepie,
                this, [this](int id){
                    emit openRecipe(id);
                });
    }

    updateContentGeometry();
}
void MewScrollArea::setContentItem(QGraphicsItem* item)
{
    if (!item)
        return;

    _content = dynamic_cast<QGraphicsObject*>(item);

    if (!_content)
    {
        class Wrapper : public QGraphicsObject
        {
        public:
            Wrapper(QGraphicsItem* child, QGraphicsItem* parent = nullptr)
                : QGraphicsObject(parent), _child(child)
            {
                _child->setParentItem(this);
            }

            QRectF boundingRect() const override
            {
                return _child->boundingRect();
            }

            void paint(QPainter*, const QStyleOptionGraphicsItem*, QWidget*) override
            {
            }

        private:
            QGraphicsItem* _child;
        };

        _content = new Wrapper(item, this);
    }
    else
    {
        _content->setParentItem(this);
    }

    _content->setPos(0, 0);
    _content->installSceneEventFilter(this);   // <<< и здесь

    updateContentGeometry();
}



void MewScrollArea::updateContentGeometry()
{
    if (!_content)
        return;

    QRectF br = _content->boundingRect();

    _contentWidth  = br.width();
    _contentHeight = br.height();

    // ограничиваем scrollY
    qreal maxScroll = _viewport.height() - _contentHeight;

    if (_scrollY > 0)
         _scrollY = 0;

    if (_scrollY < maxScroll)
          _scrollY = maxScroll;

    _content->setPos(0, _scrollY);

    update();
}

void MewScrollArea::scrollToBottom()
{
    if (!_content)
        return;

    qreal maxScroll = _viewport.height() - _contentHeight;
    _scrollY = maxScroll;

    _content->setPos(0, _scrollY);
}

void MewScrollArea::scrollToTop()
{
    _scrollY = 0;
    if (_content)
        _content->setPos(0, _scrollY);
}

void MewScrollArea::wheelEvent(QGraphicsSceneWheelEvent* event)
{
    if (!_content)
        return;

    _scrollY += event->delta() / 2.0;

    qreal maxScroll = _viewport.height() - _contentHeight;

    if (_scrollY > 0)
        _scrollY = 0;

    if (_scrollY < maxScroll)
        _scrollY = maxScroll;

    _content->setPos(0, _scrollY);
}

void MewScrollArea::ensureVisibleY(qreal y)
{
    if (!_content)
        return;

    // нижняя граница видимой области
    qreal visibleBottom = -_scrollY + _viewport.height();

    // если курсор ниже видимой области — прокручиваем вниз
    if (y > visibleBottom - 40)  // 40px запас
    {
        scrollToBottom();
        return;
    }

    // если курсор выше видимой области — прокручиваем вверх
    if (y < -_scrollY + 40)
    {
        scrollToTop();
        return;
    }
}

void MewScrollArea::setViewportHeight(qreal h)
{
    prepareGeometryChange();
    _viewport.setHeight(h);
    updateContentGeometry();
}

