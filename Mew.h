#pragma once

#include <QGraphicsScene>
#include <QGraphicsView>
#include "Interface/ColorScheme.h"
#include "Interface/Pages/HomePage.h"
#include <QResizeEvent>

class Mew: public QGraphicsView
{
public:
    Mew();
    void meow();

private:
    ColorScheme *scheme;
    QGraphicsScene *scene;
    HomePage *page1;
    void display_home_page();
    void resizeEvent(QResizeEvent* event);

};

