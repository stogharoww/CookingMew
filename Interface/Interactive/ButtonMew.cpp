#include "ButtonMew.h"

ButtonMew::ButtonMew() {

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
    _text = new QGraphicsTextItem(text, this);
    QRectF rect = _pixmap->boundingRect();
    _text->setPos(rect.x() + 5, 0);
    _text->setDefaultTextColor(_second_color);
}



void ButtonMew::set_form()
{

}
