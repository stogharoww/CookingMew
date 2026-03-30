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
    display_home_page();


    QRect screen = QApplication::primaryScreen()->availableGeometry();
    setGeometry(screen);
    setFixedSize(screen.size());
    showMaximized();

}


void Mew::meow()
{
    show();
}

void Mew::display_home_page()
{
    // ButtonMew *mew = new ButtonMew(*scheme);
    // mew->setPos(100, 100);
    // QString text = "text";
    // mew->set_text(text);
    // scene->addItem(mew);

    scene->setBackgroundBrush(scheme->backgroundGet());

    page1 = new HomePage(*scheme, QRectF(0,0,100,100));
    //page1->update_color_scheme(&scheme);
    scene->addItem(page1);
}

void Mew::resizeEvent(QResizeEvent* event)
{
    QGraphicsView::resizeEvent(event);

    QSize size = event->size();



    //setFixedSize(size);
    scene->setSceneRect(0, 0, size.width(), size.height());

    if (page1)
        page1->resize(size.width(), size.height());

}

