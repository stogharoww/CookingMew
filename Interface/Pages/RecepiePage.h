#pragma once

#include "../Virtual/Page.h"
#include "../ColorScheme.h"
#include "../Interactive/ButtonMew.h"
#include "../Interactive/MewScrollArea.h"
#include <QGraphicsTextItem>
#include <QGraphicsRectItem>
#include "../../Database/database.h"
#include <QVector>

class RecepiePage : public Page
{
    Q_OBJECT

public:
    RecepiePage(ColorScheme& scheme, QRectF rect);

    void setRecipeID(int id)        { _recipeID = id; }
    void setDatabase(DataBase* db_) { db = db_; }

    void update_pages() override;


    QVector<QString> getContent();

protected:
    void create_main_pannel() override;
    void create_right_pannel() override;

    DataBase*   db          = nullptr;
    ColorScheme _scheme;

    int _recipeID = -1;

    // данные рецепта
    QString _title;
    QString _category;
    QString _steps;
    QString _ingredients;

    // левая панель
    QGraphicsRectItem* mainRectItem = nullptr;

    QGraphicsTextItem* titleItem    = nullptr;
    QGraphicsTextItem* categoryItem = nullptr;
    QGraphicsTextItem* stepsText    = nullptr;

    AutoGrowBox*   stepsBox     = nullptr;
    MewScrollArea* stepsScroll  = nullptr;

    ButtonMew* editBtn   = nullptr;
    ButtonMew* deleteBtn = nullptr;

    // правая панель
    QGraphicsRectItem* rightRectItem       = nullptr;
    QGraphicsTextItem* ingredientsViewText = nullptr;

    AutoGrowBox*   ingredientsBox    = nullptr;
    MewScrollArea* ingredientsScroll = nullptr;
    AutoGrowBox* titleBox = nullptr;
    MewScrollArea* titleScroll = nullptr;
};
