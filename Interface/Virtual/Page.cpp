#include "Page.h"
#include <QPen>
#include <QTimer>

Page::Page(ColorScheme& scheme, QRectF rect, PageID id)
    : scheme(scheme),
    rect(rect),
    pageID(id)
{
    _btms.resize(7);
    width = rect.width();
    height = rect.height();

    textForBtm = {
        "Home", "Explore", "Bookmarks", "Ingredients",
        "My groups", "More", "Post"
    };
}

void Page::resize(int width, int height)
{
    this->width = width;
    this->height = height;
    rect = QRectF(0, 0, width, height);

    update_pages();
}

void Page::update_pages()
{
    auto items = childItems();
    for (auto* item : items)
        delete item;

    _btms.clear();
    _btms.resize(7);

    create_left_pannel();
    create_main_pannel();
    create_right_pannel();
}

void Page::create_left_pannel()
{
    // Лого
    QPixmap logoPix(":/Logo/resourses/logo/Logo.png");
    QPixmap newLogoSize = logoPix.scaled(60, 60, Qt::KeepAspectRatio, Qt::SmoothTransformation);

    QGraphicsPixmapItem *logo = new QGraphicsPixmapItem(newLogoSize);
    logo->setParentItem(this);
    logo->setPos(25, 25);

    // Линия
    Line* lines = new Line(scheme);
    lines->addLine(QPointF(width / 5, 0), QPointF(width / 5, height));
    lines->setParentItem(this);

    // Панель
    leftRect = QRectF(QPointF(0, 0), QPointF(width / 5, height));

    QGraphicsRectItem *leftPannelRect = new QGraphicsRectItem(leftRect, this);
    leftPannelRect->setAcceptedMouseButtons(Qt::NoButton);
    leftPannelRect->setPen(QPen(Qt::NoPen));

    qreal yPosBtm = leftRect.height();

    int count = 0;
    for (int i = 0; i < _btms.size(); i++)
    {
        _btms[i] = new ButtonMew(scheme);
        auto& btm = _btms[i];

        btm->setPos(leftRect.width() / 5,
                    yPosBtm - (yPosBtm - btm->boundingRect().height() - 65 * count - yPosBtm / 5));

        btm->set_text(textForBtm[count]);

        // Цвета
        if (count != 6)
            btm->change_main_color();
        else {
            QColor base = scheme.baseColorGet();
            btm->set_main_color(base);
        }

        // Жирность активной кнопки
        if (isButtonActive(count))
            btm->set_text_bold();

        btm->setParentItem(this);
        count++;
    }

    connectBtms();
}

bool Page::isButtonActive(int index)
{
    switch (pageID)
    {
    case PageID::home:        return index == 0;
    case PageID::explore:     return index == 1;
    case PageID::bookmarks:   return index == 2;
    case PageID::ingredients: return index == 3;
    case PageID::myGroups:    return index == 4;
    case PageID::more:        return index == 5;
    case PageID::post:        return index == 6;
    default: return false;
    }
}

void Page::btmHomeClicked()        { emit changeCurrentPage(PageID::home); }
void Page::btmExploreClicked()     { emit changeCurrentPage(PageID::explore); }
void Page::btmBookmarksClicked()   { emit changeCurrentPage(PageID::bookmarks); }
void Page::btmIngredientsClicked() { emit changeCurrentPage(PageID::ingredients); }
void Page::btmMyGroupsClicked()    { emit changeCurrentPage(PageID::myGroups); }
void Page::btmMoreClicked()        { emit changeCurrentPage(PageID::more); }
void Page::btmPostClicked()        { emit changeCurrentPage(PageID::post); }


void Page::connectBtms()
{
    int count = 0;
    for (auto& btm : _btms)
    {
        switch (count)
        {
        case 0: connect(btm, &Button::clicked, this, &Page::btmHomeClicked); break;
        case 1: connect(btm, &Button::clicked, this, &Page::btmExploreClicked); break;
        case 2: connect(btm, &Button::clicked, this, &Page::btmBookmarksClicked); break;
        case 3: connect(btm, &Button::clicked, this, &Page::btmIngredientsClicked); break;
        case 4: connect(btm, &Button::clicked, this, &Page::btmMyGroupsClicked); break;
        case 5: connect(btm, &Button::clicked, this, &Page::btmMoreClicked); break;
        case 6: connect(btm, &Button::clicked, this, &Page::btmPostClicked); break;
        }
        count++;
    }
}
