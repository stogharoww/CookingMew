#pragma once
#include <QColor>
#include "../ColorScheme.h"
#include "../Items/IngridientsList.h"
#include "../Virtual/Button.h"
#include "../Interactive/ButtonMew.h"
#include <QVector>
#include <QString>
#include <QGraphicsItemGroup>
#include <QGraphicsItem>
#include "../Items/Line.h"
#include <QRectF>


class Page: public QGraphicsItemGroup
{
public:
    Page(ColorScheme scheme, QRectF rect);


    virtual void create_left_pannel();
protected:
    //virtual void create_central_panel();
    //virtual void create_right_panel();
    virtual QRectF boundingRect() const override;

    ColorScheme scheme;
    QRectF rect;
    qreal width;
    qreal height;


};
