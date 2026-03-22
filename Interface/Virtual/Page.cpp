#include "Page.h"


Page::Page(ColorScheme scheme, QRectF rect)
    : scheme(scheme),
    rect(rect)
{
    width = rect.width();
    height = rect.height();
    create_left_pannel();
    //create_central_panel();
    //create_right_panel();
    setPos(rect.topLeft());
    setFlag(QGraphicsItem::ItemHasNoContents, false);
}

void Page::create_left_pannel()
{
    Line* lines = new Line(scheme);

    lines->addLine(QPointF(0, 0), QPointF(0, height));
    lines->setPos(width * 0.25, 0);

    addToGroup(lines);
}


QRectF Page::boundingRect() const
{
    return rect;
}
