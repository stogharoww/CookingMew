#include "SearchMew.h"


SearchMew::SearchMew(ColorScheme &scheme, QGraphicsItem *parent)
    : QGraphicsObject(parent),
        _scheme(scheme)
{
    setupInput();
}

void SearchMew::setupInput()
{
    _input = new QLineEdit();
    _input->setPlaceholderText("Поиск...");
    _input->setFixedSize(180, 30);

    _proxy = new QGraphicsProxyWidget(this);
    _proxy->setWidget(_input);
    _proxy->setPos(10, 10);
    //_proxy->resize(180, 30);
}

QRectF SearchMew::boundingRect() const
{
    return QRectF(0, 0, 200, 100);
}

void SearchMew::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
    painter->setRenderHint(QPainter::Antialiasing);
    painter->setBrush(QColor(60, 60, 60));
    painter->setPen(QPen(QColor(200, 200, 200), 2));
    painter->drawRoundedRect(0, 0, 200, 100, 10, 10);
}
