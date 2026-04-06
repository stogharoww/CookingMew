#include "Page.h"
#include <QPen>
#include <QTimer>

Page::Page(ColorScheme& scheme, QRectF rect)
    : scheme(scheme),
    rect(rect)
{
    _btms.resize(7);
    width = rect.width();
    height = rect.height();
    // _btms = {
    //     homeBtm, exploreBtm, bookmarksBtm, ingredientsBtm,
    //     myGroupsBtm, moreBtm, postBtm
    // };
    textForBtm = {
        "Home", "Explore", "Bookmarks", "Ingredients",
        "My groups", "More", "Post"
    };
    currentBtm = textForBtm[0];
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
    // Кнопки
    int count = 0;

    for (int i = 0; i < _btms.size(); i++) {
        _btms[i] = new ButtonMew(scheme);
        auto& btm = _btms[i];
        btm->setPos(leftRect.width() / 5, yPosBtm -
                            (yPosBtm - btm->boundingRect().height() - 65 * count - yPosBtm / 5));
        btm->set_text(textForBtm[count]);
        if (count != 6)
            btm->change_main_color();
        if (count == 6)
            btm->set_text_bold();
        if (textForBtm[count] == currentBtm){
            btm->set_text_bold();
        }
        count++;
        btm->setParentItem(this);
    }

    // button = new ButtonMew(scheme);
    // QString text = "text";
    // button->set_text(text);
    // button->setPos(leftRect.width() / 3.25, leftRect.height() / 5);
    // button->setZValue(999);
    // button->setParentItem(this);
    connectBtms();
}

void Page::resize(int width, int height)
{
    this->width = width;
    this->height = height;
    rect = QRectF(0, 0, width, height);

    // Удаляем ВСЕ дочерние элементы

    update_pages();
}

// void Page::update_color_scheme(ColorScheme &new_scheme)
// {
//     scheme = new_scheme;
//     update();
// }

void Page::update_pages()
{
    auto items = childItems();
    QTimer::singleShot(0, this, [this, items]() {
        for (auto* item : items)
            delete item;
        _btms.clear();
        _btms.resize(7);
        create_left_pannel();
        create_main_pannel();
        create_right_pannel();
    });
}

void Page::change_current_btm(QString &btmText)
{
    currentBtm = btmText;
    update_pages();

}



void Page::btmHomeClicked()
{
    qDebug() << "clicked";
    change_current_btm(textForBtm[0]);
    emit goToHome();
}

void Page::btmExploreClicked()
{
    change_current_btm(textForBtm[1]);
    emit goToExplore();
}

void Page::btmBookmarksClicked()
{
    change_current_btm(textForBtm[2]);
    emit goToBookmarks();
}

void Page::btmIngredientsClicked()
{
    change_current_btm(textForBtm[3]);
    emit goToIngredients();
}

void Page::btmMyGroupsClicked()
{
    change_current_btm(textForBtm[4]);
    emit goToMyGroups();
}

void Page::btmMoreClicked()
{
    change_current_btm(textForBtm[5]);
    emit goToMore();
}

void Page::btmPostClicked()
{
    change_current_btm(textForBtm[6]);
    emit openPost();
}

void Page::connectBtms()
{
    int cout = 0;
    for (auto& btm : _btms){
        switch (cout){
        case 0:
            connect(btm, &Button::clicked, this, &Page::btmHomeClicked);
            break;
        case 1:
            connect(btm, &Button::clicked, this, &Page::btmExploreClicked);
            break;
        case 2:
            connect(btm, &Button::clicked, this, &Page::btmBookmarksClicked);
            break;
        case 3:
            connect(btm, &Button::clicked, this, &Page::btmIngredientsClicked);
            break;
        case 4:
            connect(btm, &Button::clicked, this, &Page::btmMyGroupsClicked);
            break;
        case 5:
            connect(btm, &Button::clicked, this, &Page::btmMoreClicked);
            break;
        case 6:
            connect(btm, &Button::clicked, this, &Page::btmPostClicked);
            break;
        }
        cout++;
    }
}
