#pragma once

#include "../Virtual/Button.h"
#include <QGraphicsTextItem>
#include <QGraphicsRectItem>
#include <QPen>
#include <QBrush>
#include <QPainter>

class MewItem : public Button
{
    Q_OBJECT
public:
    MewItem(ColorScheme& scheme, QRectF& globalRect, QGraphicsItem* parent = nullptr);
    void setRecepieID(int id);
    void setContent(const QString& title,
                    const QString& group,
                    const QString& ingredients,
                    const QString& steps,
                    const QString& tag);

    QRectF boundingRect() const override;
    void paint(QPainter*, const QStyleOptionGraphicsItem*, QWidget*) override;

protected:
    void hoverEnterEvent(QGraphicsSceneHoverEvent* event) override;
    void hoverLeaveEvent(QGraphicsSceneHoverEvent* event) override;

signals:
    void clicked(int recID);

private:
    int _recepieID = -1;
    ColorScheme& _scheme;

    QRectF mainRect;

    QGraphicsTextItem* titleItem;
    QGraphicsTextItem* groupItem;
    QGraphicsTextItem* ingredientsItem;
    QGraphicsTextItem* stepsItem;
    QGraphicsTextItem* tagItem;

    qreal maxIngredientsHeight = 120;
    qreal maxStepsHeight = 120;

    bool hovered = false;

    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;
};
