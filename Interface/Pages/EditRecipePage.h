#pragma once

#include "../Virtual/Page.h"
#include "../Items/IngredientRow.h"
#include "../../Database/database.h"
#include "../ColorScheme.h"
#include "../Interactive/MewScrollArea.h"

#include <tuple>   // std::tuple

class EditRecipePage : public Page
{
    Q_OBJECT

public:
    EditRecipePage(ColorScheme& scheme, QRectF rect);

    void setContent(QVector<QString> content);
    void update_pages() override;

    void setDatabase(DataBase* database) { db = database; }
    void setRecipeID(int id) { _recipeID = id; needRebuild = true; }

signals:
    void goBackToRecipe(int id);

private:
    // Page
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

    // Data
    QString _title;
    QString _category;
    QString _steps;
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

    // SQL helpers
    bool loadRecipeFromSql();
    void loadIngredientsFromSql(QVector<std::tuple<QString,double,int>>& out);

    int  findOrCreateIngredientSql(const QString& name);
    void deleteIngredientsForRecipeSql(int recipeId);
    void insertIngredientRowSql(int recipeId, int ingredientId, double amount, int unitId);

    ColorScheme _scheme;
    bool needRebuild = false;
    ButtonMew* addIngredientBtn = nullptr;
    ButtonMew* removeIngredientBtn = nullptr;
    void addIngredientRow();
    void removeLastIngredientRow();
    void repositionIngredientRows();

    MewScrollArea* ingredientsScroll = nullptr;
    QGraphicsRectItem* ingredientsContainer = nullptr;



};
