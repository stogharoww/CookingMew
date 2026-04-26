#include "MewItem.h"
#include <QFont>
#include <QPainter>

MewItem::MewItem(ColorScheme &scheme, QRectF &globalRect, QGraphicsItem *parent)
    : Button(scheme, parent),
    _scheme(scheme)
{
    setAcceptHoverEvents(true);

    // фиксированная высота карточки
    mainRect = QRectF(4, 4, globalRect.width() - 8, 500);
    setBoundingRect(mainRect);

    // ====== GROUP ======
    groupItem = new QGraphicsTextItem(this);
    groupItem->setDefaultTextColor(_scheme.groupColor());
    groupItem->setFont(QFont("Arial", 22));
    groupItem->setPos(20, 20);

    // ====== TITLE ======
    titleItem = new QGraphicsTextItem(this);
    titleItem->setDefaultTextColor(_scheme.titleColor());
    titleItem->setFont(QFont("Arial", 14));
    titleItem->setPos(25, 60);

    // ====== INGREDIENTS ======
    ingredientsItem = new QGraphicsTextItem(this);
    ingredientsItem->setDefaultTextColor(_scheme.additionalColorGet());
    ingredientsItem->setFont(QFont("Arial", 12));
    ingredientsItem->setTextWidth(mainRect.width() - 40);
    ingredientsItem->setPos(25, 90);

    // ====== TAG ======
    tagItem = new QGraphicsTextItem(this);
    tagItem->setDefaultTextColor(_scheme.tagColor());
    tagItem->setFont(QFont("Arial", 12));
    tagItem->setPos(groupItem->boundingRect().height() + 25, 25);

    // ====== STEPS ======
    stepsItem = new QGraphicsTextItem(this);
    stepsItem->setDefaultTextColor(_scheme.textColorGet());
    stepsItem->setFont(QFont("Arial", 12));
    stepsItem->setTextWidth(mainRect.width() - 40);
    stepsItem->setPos(25, 250);
}

void MewItem::paint(QPainter* painter,
                    const QStyleOptionGraphicsItem*,
                    QWidget*)
{
    painter->setRenderHint(QPainter::Antialiasing);

    // ====== ТЕНЬ ======
    painter->setPen(Qt::NoPen);
    painter->setBrush(QColor(0, 0, 0, hovered ? 70 : 40)); // тень усиливается при наведении
    painter->drawRoundedRect(mainRect.adjusted(3, 3, 3, 3), 8, 8);

    // ====== СЕРЫЙ ФОН ======
    QColor bg = hovered
                    ? QColor(70, 70, 70)   // серый при наведении
                    : QColor(40, 40, 40);  // обычный тёмно‑серый фон

    painter->setBrush(bg);
    painter->setPen(QPen(_scheme.borderGet(), 1));
    painter->drawRoundedRect(mainRect, 8, 8);
}

void MewItem::hoverEnterEvent(QGraphicsSceneHoverEvent*)
{
    hovered = true;
    update();
}

void MewItem::hoverLeaveEvent(QGraphicsSceneHoverEvent*)
{
    hovered = false;
    update();
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
    tagItem->setPlainText("@" + tag);

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
