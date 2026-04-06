#pragma once
#include <QGraphicsObject>
#include <QGraphicsItem>
#include <QGraphicsRectItem>
#include <QPainter>
#include <QPen>
#include <QGraphicsSceneWheelEvent>
#include <QRectF>


class MewScrollArea : public QGraphicsObject
{
    Q_OBJECT

public:
    explicit MewScrollArea(QRectF viewportRect, QGraphicsItem* parent = nullptr);

    void setContent(QGraphicsItem* item);

    QRectF boundingRect() const override;
    void paint(QPainter* painter,
               const QStyleOptionGraphicsItem* option,
               QWidget* widget) override;

protected:
    void wheelEvent(QGraphicsSceneWheelEvent* event) override;

private:
    QRectF _viewport;
    QGraphicsItem* _content = nullptr;
    qreal _scrollY = 0;
};


