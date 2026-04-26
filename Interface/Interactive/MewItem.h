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

    void setContent(const QString& title,
                    const QString& group,
                    const QString& ingredients,
                    const QString& steps,
                    const QString& tag);

    QRectF boundingRect() const override;
    void paint(QPainter*, const QStyleOptionGraphicsItem*, QWidget*) override;

    void setRecepieID(int id);

signals:
    void clicked(int recID);


private:
    int _recepieID = -1;
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
    qreal maxStepsHeight = 350;
    qreal maxStepsWeight = 60;

    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;
};
