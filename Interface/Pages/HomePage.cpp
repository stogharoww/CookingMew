#include "HomePage.h"
#include "../Interactive/ButtonIcon.h"
#include <QTimer>
#include "../Interactive/SquareTextBtm.h"
#include <QPen>
#include <QGraphicsRectItem>


HomePage::HomePage(ColorScheme& scheme, QRectF rect)
    : Page(scheme, rect)
{


}

void HomePage::create_main_pannel()
{
    //центральная панель:
    mainRect = QRectF(0, 0, width / 1.8, height + 1);
    QGraphicsRectItem *mainRectItem = new QGraphicsRectItem(mainRect);
    mainRectItem->setPos(leftRect.width(), 0);
    mainRectItem->setParentItem(this);
    mainRectItem->setPen(QPen(scheme.borderGet(), 1));

    //сердечко
    ButtonIcon *icon = new ButtonIcon(ButtonType::Like, scheme, mainRectItem, QColor(Qt::red));
    icon->setParentItem(mainRectItem);
    icon->setPos(100, 100);

    //верхняя панель:
    SquareTextBtm *forYouBtm = new SquareTextBtm(scheme, mainRectItem);
    forYouBtm->setParentItem(mainRectItem);
    forYouBtm->setPos(0, 0);
    forYouBtm->setSize(mainRect.width() / 2, 40);
    QString forYouName = "For you";
    forYouBtm->setText(forYouName);

    SquareTextBtm *followBtm = new SquareTextBtm(scheme, mainRectItem);
    followBtm->setParentItem(mainRectItem);
    followBtm->setPos(mainRect.width() / 2, 0);
    followBtm->setSize(mainRect.width() / 2, 40);
    QString followName = "Follow";
    followBtm->setText(followName);


}

