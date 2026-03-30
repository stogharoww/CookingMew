#pragma once

#include <QGraphicsObject>
#include "../Interactive/MewItem.h"
#include "../ColorScheme.h"
#include <QRectF>
#include <QVector>

class FollowLent : public QGraphicsObject
{
    Q_OBJECT
public:
    FollowLent(ColorScheme& scheme, QRectF rect, QGraphicsItem* parent = nullptr);

    QRectF boundingRect() const override;
    void paint(QPainter*, const QStyleOptionGraphicsItem*, QWidget*) override;

    void addPost(MewItem* post);

private:
    ColorScheme _scheme;
    QRectF globalRect;
    QRectF _bounds;
    QVector<MewItem*> posts;
};
