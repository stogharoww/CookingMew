#include "Button.h"


void Button::set_color(QColor &col)
{
    _color = col;
    _brush.setColor(_color);
    setBrush(_brush);
}


void Button::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    event->accept();
    emit clicked();
}


void Button::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    event->accept();
}


void Button::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    event->accept();
    change_color();
}


void Button::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    event->accept();
    change_color();
}


void Button::change_color()
{
    _changed = !_changed;
    if (_changed == false)
        _brush.setColor(_color);
    if (_changed == true)
        _brush.setColor(_color.lighter(150));
    setBrush(_brush);
}
