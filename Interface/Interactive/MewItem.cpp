#include "MewItem.h"
#include <QFont>
#include <QPainter>




MewItem::MewItem(ColorScheme &scheme, QRectF &globalRect, QGraphicsItem *parent)
    : Button(scheme, parent),
    _scheme(scheme)
{
    // фиксированная высота карточки
    mainRect = QRectF(4, 4, globalRect.width() - 8, 500);
    setBoundingRect(mainRect);

    // ====== TITLE ======
    titleItem = new QGraphicsTextItem(this);
    titleItem->setDefaultTextColor(_scheme.titleColor());
    titleItem->setFont(QFont("Arial", 22));
    titleItem->setPos(20, 20);
    titleItem->setAcceptedMouseButtons(Qt::NoButton);
    titleItem->setAcceptHoverEvents(false);

    // ====== GROUP ======
    groupItem = new QGraphicsTextItem(this);
    groupItem->setDefaultTextColor(_scheme.groupColor());
    groupItem->setFont(QFont("Arial", 14));
    groupItem->setPos(20, 60);
    groupItem->setAcceptedMouseButtons(Qt::NoButton);
    groupItem->setAcceptHoverEvents(false);

    // ====== INGREDIENTS ======
    ingredientsItem = new QGraphicsTextItem(this);
    ingredientsItem->setDefaultTextColor(_scheme.textColorGet());
    ingredientsItem->setFont(QFont("Arial", 12));
    ingredientsItem->setTextWidth(mainRect.width() - 40);
    ingredientsItem->setPos(20, 90);
    ingredientsItem->setAcceptedMouseButtons(Qt::NoButton);
    ingredientsItem->setAcceptHoverEvents(false);

    // ====== TAG ======
    tagItem = new QGraphicsTextItem(this);
    tagItem->setDefaultTextColor(_scheme.tagColor());
    tagItem->setFont(QFont("Arial", 12));
    tagItem->setPos(20, 220);
    tagItem->setAcceptedMouseButtons(Qt::NoButton);
    tagItem->setAcceptHoverEvents(false);

    // ====== STEPS ======
    stepsItem = new QGraphicsTextItem(this);
    stepsItem->setDefaultTextColor(_scheme.textColorGet());
    stepsItem->setFont(QFont("Arial", 12));
    stepsItem->setTextWidth(mainRect.width() - 40);
    stepsItem->setPos(20, 250);
    stepsItem->setAcceptedMouseButtons(Qt::NoButton);
    stepsItem->setAcceptHoverEvents(false);
}

void MewItem::paint(QPainter* painter,
                    const QStyleOptionGraphicsItem*,
                    QWidget*)
{
    painter->setRenderHint(QPainter::Antialiasing);

    // фон карточки — управляется Button::_brush
    painter->setPen(QPen(_scheme.borderGet(), 1));
    painter->setBrush(_brush);
    painter->drawRect(mainRect);
}

void MewItem::setContent(const QString& title,
                         const QString& group,
                         const QString& ingredients,
                         const QString& steps,
                         const QString& tag)
{
    titleItem->setPlainText(title);
    groupItem->setPlainText(group);
    ingredientsItem->setPlainText(ingredients);
    stepsItem->setPlainText(steps);
    tagItem->setPlainText(tag);

    // ====== Обрезка ингредиентов ======
    if (ingredientsItem->boundingRect().height() > maxIngredientsHeight)
    {
        QString t = ingredients;
        while (ingredientsItem->boundingRect().height() > maxIngredientsHeight && t.size() > 10)
        {
            t.chop(10);
            ingredientsItem->setPlainText(t + "...");
        }
    }

    // ====== Обрезка шагов ======
    if (stepsItem->boundingRect().height() > maxStepsHeight)
    {
        QString t = steps;
        while (stepsItem->boundingRect().height() > maxStepsHeight && t.size() > 10)
        {
            t.chop(10);
            stepsItem->setPlainText(t + "...");
        }
    }
}

QRectF MewItem::boundingRect() const
{
    return mainRect;
}
