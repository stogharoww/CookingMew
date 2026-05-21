#pragma once

#include "../Virtual/Page.h"
#include "../Items/IngredientRow.h"
#include "../../Database/database.h"
#include "../ColorScheme.h"

class PostPage : public Page
{
    Q_OBJECT

public:
    PostPage(ColorScheme& scheme, QRectF rect);

    void setDatabase(DataBase* database) { db = database; }

    void update_pages() override;


signals:
    void goToRecipePage(int recipeID);


private:
    // Реализация виртуальных методов Page
    void create_main_pannel() override;
    void create_right_pannel() override;

    // UI
    QGraphicsRectItem* mainRectItem  = nullptr;
    QGraphicsRectItem* rightRectItem = nullptr;

    TextEditMew* titleEdit = nullptr;
    TextEditMew* stepsEdit = nullptr;
    QGraphicsTextItem* categoryItem = nullptr;
    ButtonMew* saveBtn = nullptr;

    QVector<IngredientRow*> ingredientRows;

    // DB
    DataBase* db = nullptr;
    int _recipeID = 0;

    // Helpers
    void editMode();
    void buildMainPanel();
    void buildRightPanel();
    void buildIngredientsPanel(QGraphicsItem* parent);
    void saveRecipe();
    void saveIngredients();

    // DB helpers
    int findOrCreateIngredient(const QString& name);
    void insertIngredientRow(const Recipeingred& ri);

    ColorScheme _scheme;

    // Чтобы страница строилась только один раз
    bool needRebuild = true;


    ButtonMew* addIngredientBtn = nullptr;

    void addIngredientRow();
    void repositionIngredientRows();
    qreal ingredientsTopY = 0;

    ButtonMew* removeIngredientBtn = nullptr;

    void removeLastIngredientRow();

    int getOrCreateUserCategory();

    QGraphicsTextItem* errorLabel = nullptr;




};
