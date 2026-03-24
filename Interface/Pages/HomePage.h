#pragma once

#include "../Virtual/Page.h"
#include <QColor>
#include "../ColorScheme.h"
#include "../Items/IngridientsList.h"
#include "../Virtual/Button.h"
#include "../Interactive/ButtonMew.h"
#include <QVector>
#include <QString>
#include <QGraphicsItemGroup>
#include <QGraphicsItem>
#include "../Items/Line.h"
#include <QRectF>

class HomePage : public Page
{
public:
    HomePage(ColorScheme &scheme, QRectF rect);


private:
    void create_main_pannel() override;
};

