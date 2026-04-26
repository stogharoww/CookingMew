#include "HomePage.h"
#include "../Interactive/ButtonIcon.h"
#include <QTimer>
#include "../Interactive/SquareTextBtm.h"
#include <QPen>
#include <QGraphicsRectItem>
#include <QDebug>

HomePage::HomePage(ColorScheme& scheme, QRectF rect)
    : Page(scheme, rect, PageID::home),
    _scheme(scheme)
{
    // Никаких currentBtm, никаких change_current_btm
}

void HomePage::btmForYouClicked()
{
    forYouBtm->set_text_bold();
    followBtm->set_not_text_bold();
    moveLine(forYouBtm);

    forYouScrollArea->setVisible(true);

    update();
}

void HomePage::btmFollowClicked()
{
    followBtm->set_text_bold();
    forYouBtm->set_not_text_bold();
    moveLine(followBtm);

    forYouScrollArea->setVisible(false);

    update();
}

void HomePage::btmHeartClicked()
{
    // пока пусто
}

void HomePage::postClicked()
{
    // пока пусто
}

void HomePage::create_main_pannel()
{
    // центральная панель:
    mainRect = QRectF(0, 0, width / 1.8, height + 1);
    mainRectItem = new QGraphicsRectItem(mainRect);
    mainRectItem->setPos(leftRect.width(), 0);
    mainRectItem->setParentItem(this);
    mainRectItem->setPen(QPen(scheme.borderGet(), 1));

    // верхняя панель:
    forYouBtm = new SquareTextBtm(scheme, mainRectItem);
    forYouBtm->setParentItem(mainRectItem);
    forYouBtm->setPos(0, 0);
    forYouBtm->setSize(mainRect.width() / 2, 40);
    QString forYouName = "For you";
    forYouBtm->setText(forYouName);
    forYouBtm->set_text_bold(); // по умолчанию активная

    followBtm = new SquareTextBtm(scheme, mainRectItem);
    followBtm->setParentItem(mainRectItem);
    followBtm->setPos(mainRect.width() / 2, 0);
    followBtm->setSize(mainRect.width() / 2, 40);
    QString followName = "Follow";
    followBtm->setText(followName);

    // линия под активной кнопкой
    followAndForYouLine = new Line(_scheme);
    followAndForYouLine->changeColor(_scheme.additionalColorGet(), 3);
    followAndForYouLine->setParentItem(mainRectItem);
    moveLine(forYouBtm);

    // список кнопок верхней панели
    __btms = {
        forYouBtm,
        followBtm
    };

    connecting();

    // scroll area
    QPointF scrlATOPLEFT = QPointF(followBtm->boundingRect().bottomLeft());
    QPointF scrlBOTTOMRIGT = QPointF(mainRect.bottomRight());
    QRectF scrollRect = QRectF(scrlATOPLEFT, scrlBOTTOMRIGT);

    forYouScrollArea = new MewScrollArea(scrollRect, mainRectItem);
    FollowLent *lent = new FollowLent(scheme, scrollRect);
    forYouScrollArea->setContent(lent);
}

void HomePage::moveLine(SquareTextBtm *btm)
{
    QPointF centr = btm->mapToItem(mainRectItem, btm->boundingRect().center());
    QPointF linePosStart = QPoint(centr.x() - 30, centr.y() * 2);
    QPointF linePosEnd   = QPoint(centr.x() + 30, centr.y() * 2);

    followAndForYouLine->moveTo(linePosStart, linePosEnd);
    update();
}

void HomePage::connecting()
{
    int count = 0;
    for (auto &btm : __btms)
    {
        switch (count)
        {
        case 0:
            connect(btm, &Button::clicked, this, &HomePage::btmForYouClicked);
            break;
        case 1:
            connect(btm, &Button::clicked, this, &HomePage::btmFollowClicked);
            break;
        }
        count++;
    }
}
