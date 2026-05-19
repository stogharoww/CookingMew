#pragma once

#include "../Virtual/Page.h"
#include "../Items/IngredientRow.h"
#include "../../Database/database.h"
#include "../ColorScheme.h"

class EditRecipePage : public Page
{
    Q_OBJECT

public:
    EditRecipePage(ColorScheme& scheme, QRectF rect);

    void setContent(QVector<QString> content);
    void update_pages() override;

    // ВАЖНО: эти методы нужны PageChanger
    void setDatabase(DataBase* database) { db = database; }
    void setRecipeID(int id) { _recipeID = id; }

signals:
    void goBackToRecipe(int id);

private:
    // Реализация чисто виртуальных методов Page
    void create_main_pannel() override;
    void create_right_pannel() override;

    // UI
    QGraphicsRectItem* mainRectItem = nullptr;
    QGraphicsRectItem* rightRectItem = nullptr;

    TextEditMew* titleEdit = nullptr;
    TextEditMew* stepsEdit = nullptr;
    QGraphicsTextItem* categoryItem = nullptr;
    ButtonMew* saveBtn = nullptr;

    QVector<IngredientRow*> ingredientRows;

    // Data
    QString _title;
    QString _category;
    QString _steps;
    QString _ingredients;
    bool dataAviable = false;

    // DB
    DataBase* db = nullptr;
    int _recipeID = 0;

    // Helpers
    void editMode();
    void buildMainPanel();
    void buildRightPanel();
    void buildIngredientsPanel(QGraphicsItem* parent, qreal topY);

    void saveRecipe();
    void saveIngredients();

    // DB helpers
    QVector<Recipeingred> ingredientsForRecipe(int recipeId);
    ingredient getIngredientById(int id);
    int findOrCreateIngredient(const QString& name);
    void deleteIngredientsForRecipe(int recipeId);
    void insertIngredientRow(const Recipeingred& ri);

    ColorScheme _scheme;


};
