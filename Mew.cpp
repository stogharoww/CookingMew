#include "Mew.h"
#include "Interface/Virtual/Button.h"
#include "Interface/Interactive/ButtonMew.h"
#include <QGraphicsItem>
#include <QSize>
#include "Interface/Pages/HomePage.h"




Mew::Mew() {
    QGraphicsScene *scene = new QGraphicsScene(this);
    setScene(scene);
    showMaximized();

    scheme = new ColorScheme();

    QSize size = viewport()->size();
    qreal width = size.width();
    qreal height = size.height();

    setSceneRect(0, 0, width, height);
    ButtonMew *mew = new ButtonMew(*scheme);
    mew->setPos(-400, 400);
    QString text = "text";
    //mew->setColorScheme(colorScheme->getThemeColor());
    mew->set_text(text);
    //mew->change_main_color();
    scene->addItem(mew);
    QColor backgroundColor = scheme->getThemeColor()[5];
    scene->setBackgroundBrush(backgroundColor);

    HomePage *page1 = new HomePage(scheme, scene->sceneRect());
    scene->addItem(page1);
    page1->create_left_pannel();

}
