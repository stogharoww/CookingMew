#include "Page.h"
#include <QPen>

Page::Page(ColorScheme& scheme, QRectF rect)
    : scheme(scheme),
    rect(rect)
{
    width = rect.width();
    height = rect.height();
    _btms = {
        homeBtm, exploreBtm, bookmarksBtm, ingredientsBtm,
        myGroupsBtm, moreBtm, postBtm
    };
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

    qreal yPosBtm = leftRect.height();
    // Кнопки
    int count = 0;
    QVector<QString> textForBtm = {
        "Home", "Explore", "Bookmarks", "Ingredients",
        "My groups", "More", "Post"
    };
    for (auto& btm : _btms){
        btm = new ButtonMew(scheme);
        btm->setPos(leftRect.width() / 3.25, yPosBtm -
                            (yPosBtm - btm->boundingRect().height() - 65 * count - yPosBtm / 5));
        btm->set_text(textForBtm[count]);
        count++;
        btm->setParentItem(this);
    }

    // button = new ButtonMew(scheme);
    // QString text = "text";
    // button->set_text(text);
    // button->setPos(leftRect.width() / 3.25, leftRect.height() / 5);
    // button->setZValue(999);
    // button->setParentItem(this);
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
