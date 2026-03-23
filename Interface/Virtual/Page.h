#pragma once
#include <QGraphicsObject>
#include "../ColorScheme.h"
#include "../Items/IngridientsList.h"
#include "../Virtual/Button.h"
#include "../Interactive/ButtonMew.h"
#include "../Items/Line.h"

class Page : public QGraphicsObject
{
    Q_OBJECT

public:
    Page(ColorScheme& scheme, QRectF rect);

    virtual void create_left_pannel();
    virtual void resize(int width, int height);

    QRectF boundingRect() const override { return rect; }
    void paint(QPainter*, const QStyleOptionGraphicsItem*, QWidget*) override {}
    //virtual void update_color_scheme(ColorScheme &new_scheme);

protected:
    virtual void update_pages();

    ColorScheme& scheme;
    QRectF rect;
    qreal width;
    qreal height;

    ButtonMew *button;
    ButtonMew *homeBtm;
};
