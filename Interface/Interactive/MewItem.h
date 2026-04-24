#pragma once

#include "../Virtual/Button.h"
#include <QGraphicsTextItem>
#include <QGraphicsRectItem>
#include <QPen>
#include <QBrush>
#include <QPainter>

class MewItem : public Button
{
public:
    MewItem(ColorScheme& scheme, QRectF& globalRect, QGraphicsItem* parent = nullptr);

    void setContent(const QString& title,
                    const QString& group,
                    const QString& ingredients,
                    const QString& steps,
                    const QString& tag);

    QRectF boundingRect() const override;
    void paint(QPainter*, const QStyleOptionGraphicsItem*, QWidget*) override;

private:
    ColorScheme& _scheme;

    QRectF mainRect;

    // элементы карточки
    QGraphicsTextItem* titleItem;
    QGraphicsTextItem* groupItem;
    QGraphicsTextItem* ingredientsItem;
    QGraphicsTextItem* stepsItem;
    QGraphicsTextItem* tagItem;

    // ограничения
    qreal maxIngredientsHeight = 120;
    qreal maxStepsHeight = 120;
};
