#pragma once

#include "../Virtual/Page.h"
#include "../ColorScheme.h"
#include "../Interactive/MewScrollArea.h"
#include <QGraphicsTextItem>
#include <QGraphicsRectItem>
#include <QLineEdit>
#include <QTextEdit>
#include <QPushButton>
#include "../../Database/database.h"

class RecepiePage : public Page
{
public:
    RecepiePage(ColorScheme &scheme, QRectF rect);

    void setRecipeID(int id) { _recipeID = id; }
    void setDatabase(DataBase* database) { db = database; }

private:
    void create_main_pannel() override;

    DataBase* db = nullptr;   // <--- ДОБАВЛЕНО

    ColorScheme _scheme;
    int _recipeID = -1;

    QGraphicsRectItem* mainRectItem = nullptr;
};
