#include "Button.h"
#include <QPen>
#include <QDebug>



QRectF Button::boundingRect() const
{
    return _b_rect;
}


void Button::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    currentColor = pressedColor;
    _brush.setColor(currentColor);
    update();
}


void Button::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    currentColor = hoverColor; // или baseColor
    _brush.setColor(currentColor);
    emit clicked();
    update();
}



void Button::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    //qDebug() << "Button:: hover event";
    currentColor = hoverColor;
    _brush.setColor(currentColor);
    update();
}



void Button::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    currentColor = baseColor;
    _brush.setColor(currentColor);
    update();
}


void Button::change_main_color()
{
    // Меняем местами основной и дополнительный цвет
    baseColor = backgroundColor;

    // Пересчитываем производные цвета
    hoverColor   = baseColor.lighter(175);
    pressedColor = baseColor;

    // Текущий цвет обновляем
    currentColor = baseColor;

    // Обновляем кисти
    _brush.setColor(currentColor);
    _second_brush.setColor(secondColor);

    update();
}

void Button::updateScheme(ColorScheme &colorScheme)
{
    scheme = colorScheme;
    setColorScheme();
}

void Button::set_text_bold()
{
    font.setBold(true);
}





// void Button::change_color()
// {
//     _changed = !_changed;
//     //_main_color = _main_color.toHsv();
//     if (_changed == false){
//         _main_color = _mc_tmp.lighter(100);
//         _brush.setColor(_main_color);
//     }
//     if (_changed == true){
//         if (_main_color == Qt::black)
//             _main_color.setHsv(_main_color.hue(), _main_color.saturation(), 50);
//         else
//             _main_color = _mc_tmp.lighter(150);
//         _brush.setColor(_main_color);
//     }
//     update();
//     //setBrush(_brush);
// }

void Button::setBoundingRect(QRectF &rect)
{
    _b_rect = rect;
}

void Button::setColorScheme()
{
    QVector<QColor> colorScheme = scheme.getThemeColor();
    if (colorScheme.size() < 4){
        //qDebug() << "Button::setColorScheme: error: colorScheme must contain at least 4 elements";
        return;
    }
    std::tie(baseColor, secondColor, hoverColor, pressedColor, backgroundColor) =
        std::make_tuple(colorScheme[0], colorScheme[1], colorScheme[2], colorScheme[3], colorScheme[5]);

    currentColor = baseColor;
    _brush.setColor(currentColor);
    _second_brush.setColor(secondColor);

    update();
}



void Button::setBrush(QBrush &brush)
{
    _brush = brush;
    update();
}

void Button::setSecondBrush(QBrush &brush)
{
    _second_brush = brush;
    update();
}

