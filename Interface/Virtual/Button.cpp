#include "Button.h"


void Button::set_color(QColor &col)
{
    _main_color = col;
    _brush.setColor(_main_color);
    setBrush(_brush);
}

void Button::set_second_color(QColor &col)
{
    _second_color = col;
    _second_brush.setColor(_second_color);
}


void Button::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    event->accept();
    change_color();
    emit clicked();
}


void Button::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    event->accept();
    change_color();
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
        _brush.setColor(_main_color);
    if (_changed == true)
        _brush.setColor(_main_color.lighter(150));
    setBrush(_brush);
}
