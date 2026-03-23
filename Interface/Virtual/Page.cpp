#include "Page.h"



Page::Page(ColorScheme scheme, QRectF rect)
    : scheme(scheme),
    rect(rect)
{
    width = rect.width();
    height = rect.height();
    //create_left_pannel();
    //create_central_panel();
    //create_right_panel();
    setHandlesChildEvents(true);


}

void Page::create_left_pannel()
{
    Line* lines = new Line(scheme);
    QPointF position(0, 0);
    QPointF pFrom;
    QPointF pTo;
    lines->addLine(QPointF(width / 5, 0), QPointF(width / 5, height));
    lines->setPos(position);
    QRectF leftRect(QPointF(0, 0), QPointF(width / 5, height));
    QGraphicsItem *leftPannelRect = new QGraphicsRectItem(leftRect, this);
    button = new ButtonMew(scheme, leftPannelRect);


    QString text = "text";
    button->set_text(text);
    button->setPos(leftRect.width() / 3.25, leftRect.height() / 2);
    button->setZValue(100);


    addToGroup(button);


    addToGroup(lines);
}


void Page::resize(int width, int height)
{
    this->width = width;
    this->height = height;
    rect = QRectF(0, 0, width, height);

    auto items = childItems();
    for (auto* item : items) {
        removeFromGroup(item);
        delete item;
    }
    update_pages();
}

void Page::update_pages()
{
    create_left_pannel();
}

QRectF Page::boundingRect() const
{
    return rect;
}




