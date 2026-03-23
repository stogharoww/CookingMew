#include "Page.h"
#include <QPen>

Page::Page(ColorScheme& scheme, QRectF rect)
    : scheme(scheme),
    rect(rect)
{
    width = rect.width();
    height = rect.height();
}

void Page::create_left_pannel()
{
    // Линия
    Line* lines = new Line(scheme);
    lines->addLine(QPointF(width / 5, 0), QPointF(width / 5, height));
    lines->setParentItem(this);

    // Панель
    QRectF leftRect(QPointF(0, 0), QPointF(width / 5, height));

    QGraphicsRectItem *leftPannelRect = new QGraphicsRectItem(leftRect, this);
    leftPannelRect->setAcceptedMouseButtons(Qt::NoButton);
    leftPannelRect->setPen(QPen(Qt::NoPen));


    // Кнопка
    button = new ButtonMew(scheme);
    QString text = "text";
    button->set_text(text);
    button->setPos(leftRect.width() / 3.25, leftRect.height() / 2);
    button->setZValue(999);
    button->setParentItem(this);
}

void Page::resize(int width, int height)
{
    this->width = width;
    this->height = height;
    rect = QRectF(0, 0, width, height);

    // Удаляем ВСЕ дочерние элементы
    auto items = childItems();
    for (auto* item : items)
        delete item;

    update_pages();
}

// void Page::update_color_scheme(ColorScheme &new_scheme)
// {
//     scheme = new_scheme;
//     update();
// }

void Page::update_pages()
{
    create_left_pannel();
}
