#pragma once

#include <QGraphicsScene>
#include <QGraphicsView>
#include "Interface/ColorScheme.h"
#include "Interface/Pages/HomePage.h"
#include <QResizeEvent>
#include "Database/database.h"



class Mew: public QGraphicsView
{
    Q_OBJECT
public:
    Mew();
    void meow();

public slots:
    void home();
    void recepie();

private:
    ColorScheme *scheme;
    QGraphicsScene *scene;
    HomePage *homePage;
    void display_home_page();
    void resizeEvent(QResizeEvent* event);
    bool firstResizeDone = false;

    void setUnvisibleAll();

    void initPages();
    DataBase *db;

};

