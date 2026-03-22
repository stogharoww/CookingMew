#pragma once

#include <QGraphicsScene>
#include <QGraphicsView>
#include "Interface/ColorScheme.h"

class Mew: public QGraphicsView
{
public:
    Mew();


private:
    ColorScheme *scheme;
};

