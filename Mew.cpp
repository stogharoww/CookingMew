#include "Mew.h"
#include "Interface/Virtual/Button.h"
#include "Interface/Interactive/ButtonMew.h"
#include <QGraphicsItem>
#include <QSize>
#include "Interface/Pages/HomePage.h"
#include <QTimer>
#include <QApplication>



Mew::Mew() {
    scene = new QGraphicsScene(this);
    setScene(scene);

    scheme = new ColorScheme();

    scene->setBackgroundBrush(scheme->backgroundGet());

    initPages();

    QRect screen = QApplication::primaryScreen()->availableGeometry();
    setGeometry(screen);
    setFixedSize(screen.size());
    showMaximized();

    setUnvisibleAll();
    home();


}


void Mew::meow()
{
    show();
}

void Mew::home()
{
    setUnvisibleAll();
    homePage->setVisible(true);
}

void Mew::recepie()
{
    setUnvisibleAll();
}



void Mew::resizeEvent(QResizeEvent* event)
{
    QGraphicsView::resizeEvent(event);

    QSize size = event->size();



    //setFixedSize(size);
    scene->setSceneRect(0, 0, size.width(), size.height());

    if (homePage)
        homePage->resize(size.width(), size.height());

}

void Mew::setUnvisibleAll()
{
    homePage->setVisible(false);
}

void Mew::initPages()
{
    homePage = new HomePage(*scheme, QRectF(0,0,100,100));
    //page1->update_color_scheme(&scheme);
    scene->addItem(homePage);


}

