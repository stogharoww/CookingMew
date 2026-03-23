#include "ButtonMew.h"
#include <QPen>
#include <QPainter>
#include <QStylePainter>
#include <QLinearGradient>

ButtonMew::ButtonMew(ColorScheme& scheme, QGraphicsItem* parent)
    : Button(scheme, parent)
{
    setAcceptHoverEvents(true);
    setAcceptedMouseButtons(Qt::LeftButton);

    _brush.setColor(currentColor);
    _brush.setStyle(Qt::SolidPattern);
    setBrush(_brush);

}

void ButtonMew::set_pixmap(QString &path)
{
    QPixmap original(path);

    _buttonSize = QSize(30, 30);
    QPixmap scaled = original.scaled(_buttonSize, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    _pixmap = new QGraphicsPixmapItem(scaled, this);
    qreal xPos = 0;
    qreal yPos = 0;

    _pixmap->setPos(xPos, yPos);
}

void ButtonMew::set_text(QString &text)
{
    _text = text;
    update();
}

void ButtonMew::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
    // base forms
    QPoint firstEllipse(10, 10);
    QPoint secondEllipse(110, 10);
    QPoint rectPos(10, -10);
    QSize size(100, 40);
    QRect rect(rectPos, size);
    QRectF b_rect(-10, -10, rect.width() + 40, rect.height());
    setBoundingRect(b_rect);


    painter->setRenderHint(QPainter::Antialiasing, true);
    painter->setRenderHint(QPainter::SmoothPixmapTransform, true);
    painter->setRenderHint(QPainter::TextAntialiasing, true);

    //pens
    QPen main_pen(currentColor, 2);
    QPen second_pen(secondColor, 2);

    painter->setPen(main_pen);

    //painter->setBrush(_brush);
    QLinearGradient grad(rect.topLeft(), rect.bottomLeft());
    grad.setColorAt(0.0, currentColor.lighter(130));
    grad.setColorAt(1.0, currentColor.lighter(130));
    painter->setBrush(grad);


    painter->setPen(Qt::NoPen);
    painter->drawEllipse(firstEllipse, 20, 20);
    painter->drawEllipse(secondEllipse, 20, 20);
    painter->drawRect(rect);

    painter->setPen(second_pen);
    painter->setBrush(Qt::NoBrush);
    painter->drawText(rect.width() / 2, rect.height() / 3, _text);



}



void ButtonMew::set_form()
{

}
