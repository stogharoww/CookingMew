#include "Mew.h"
#include "Interface/Virtual/Button.h"
#include "Interface/Interactive/ButtonMew.h"
#include <QGraphicsItem>
#include <QSize>
#include "Interface/Pages/HomePage.h"



Mew::Mew() {
    scene = new QGraphicsScene(this);
    setScene(scene);

    scheme = new ColorScheme();
    //scheme->changeTheme();
    display_home_page();
    showMaximized();

}

void Mew::meow()
{
    show();
}

void Mew::display_home_page()
{
    ButtonMew *mew = new ButtonMew(*scheme);
    mew->setPos(100, 100);
    QString text = "text";
    mew->set_text(text);
    scene->addItem(mew);

    scene->setBackgroundBrush(scheme->getThemeColor()[5]);

    page1 = new HomePage(scheme, QRectF(0,0,100,100));
    scene->addItem(page1);
}

void Mew::resizeEvent(QResizeEvent* event)
{
    QGraphicsView::resizeEvent(event);

    QSize size = event->size();

    scene->setSceneRect(0, 0, size.width(), size.height());

    if (page1)
        page1->resize(size.width(), size.height());

}

