#pragma once

#include "../Virtual/Page.h"
#include "../ColorScheme.h"
#include <QGraphicsTextItem>
#include <QGraphicsRectItem>

class RecepiePage : public Page
{
public:
    RecepiePage(ColorScheme &scheme, QRectF rect);

    void setRecipeID(int id) { _recipeID = id; }

private:
    void create_main_pannel() override;

    ColorScheme _scheme;
    int _recipeID = -1;

    // элементы UI
    QGraphicsRectItem* mainRectItem = nullptr;
    QGraphicsTextItem* titleItem = nullptr;
    QGraphicsTextItem* categoryItem = nullptr;
    QGraphicsTextItem* ingredientsItem = nullptr;
    QGraphicsTextItem* stepsItem = nullptr;
};
