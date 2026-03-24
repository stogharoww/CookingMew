#include "ButtonMew.h"
#include <QPen>
#include <QPainter>
#include <QStylePainter>
#include <QLinearGradient>
#include <cmath>
#include <QFontMetrics>

ButtonMew::ButtonMew(ColorScheme& scheme, QGraphicsItem* parent)
    : Button(scheme, parent)
{
    setAcceptHoverEvents(true);
    setAcceptedMouseButtons(Qt::LeftButton);

    _brush.setColor(currentColor);
    _brush.setStyle(Qt::SolidPattern);
    setBrush(_brush);
    firstEllipse = QPoint(10, 10);
    secondEllipse = QPoint(110, 10);
    rectPos = QPoint(10, -10);
    QSize size(100, 40);
    rect = QRect(rectPos, size);
    QRectF b_rect(-10, -10, rect.width() + 40, rect.height());
    setBoundingRect(b_rect);


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

    QFontMetrics fm(font);
    int textWidth = fm.horizontalAdvance(_text);

    int ellipseR = 20;      // радиус эллипсов
    int padding = 10;       // отступ слева
    int height = 40;        // высота кнопки

    // Новая ширина кнопки
    int totalWidth = padding + ellipseR*2 + textWidth + ellipseR*2;

    // boundingRect растёт только вправо
    QRectF brrect(0, 0, totalWidth, height);
    setBoundingRect(brrect);

    // Левый эллипс — всегда на одном месте
    firstEllipse = QPoint(padding + ellipseR, height / 2);

    // Прямоугольник — сразу после левого эллипса
    rect = QRect(
        padding + ellipseR,
        0,
        textWidth + ellipseR,
        height
        );

    // Правый эллипс — сразу после прямоугольника
    secondEllipse = QPoint(rect.right(), height / 2);

    update();
}



void ButtonMew::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{



    painter->setRenderHint(QPainter::Antialiasing, true);
    painter->setRenderHint(QPainter::SmoothPixmapTransform, true);
    painter->setRenderHint(QPainter::TextAntialiasing, true);

    painter->setFont(font);

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
    QRectF needRect = boundingRect();
    painter->drawText(rect, Qt::AlignCenter, _text);



}



void ButtonMew::set_form()
{

}
