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
    qDebug() << "Button:: hover event";
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

    // Пересчитываем производные цвета (динамически)
    hoverColor   = baseColor.lighter(120);
    pressedColor = baseColor.darker(120);

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

void Button::set_not_text_bold()
{
    font.setBold(false);
}





void Button::setBoundingRect(QRectF &rect)
{
    _b_rect = rect;
}

void Button::setColorScheme()
{
    if (_isCustomColor)
        return;

    QVector<QColor> colorScheme = scheme.getThemeColor();
    if (colorScheme.size() < 6){
        //qDebug() << "Button::setColorScheme: error: colorScheme must contain at least 6 elements";
        return;
    }

    // Базовые цвета из темы
    secondColor     = colorScheme[1];
    backgroundColor = colorScheme[5];
    baseColor       = backgroundColor;

    // Генерация hover/pressed динамически
    auto normalize = [](QColor c){
        if (c == Qt::black)
            return QColor(1,1,1);
        return c;
    };

    baseColor = normalize(baseColor);

    hoverColor   = baseColor.lighter(120);
    pressedColor = baseColor.darker(120);

    // Текущий цвет = базовый
    currentColor = backgroundColor;

    // Обновляем кисти
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
