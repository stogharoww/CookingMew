#pragma once

#include "../Virtual/Button.h"
#include <QVector>
#include <QString>
#include <QFont>
#include <QColor>
#include <QGraphicsItem>
#include <QGraphicsRectItem>
#include <QGraphicsTextItem>
#include <QPixmap>
#include "../ColorScheme.h"
#include <QRectF>
#include <QPainter>
#include <QPen>
#include <QGraphicsTextItem>

class MewItem : public Button
{
public:
    MewItem(ColorScheme& scheme, QRectF &globalRect, QGraphicsItem* paren = nullptr);
    void paint(QPainter* painter, const QStyleOptionGraphicsItem*, QWidget*) override;
    QRectF boundingRect() const override;



private:
    QVector<QColor> colors;

    ColorScheme _scheme;
    QPixmap _avatareGroup;
    QGraphicsRectItem *mainRectItem;
    QRectF mainRect;


    QVector<QString> content;



    void set_content(QString &title, QString &group, QString &recepie, QString &ingredients, QString &tag);

};

