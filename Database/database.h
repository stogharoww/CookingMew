#pragma once
#include <QString>
#include "categoriesrec.h"
#include "ingredients.h"
#include "recipeingred.h"


class DataBase
{
public:
    explicit DataBase(const QString& dbPath);

    [[nodiscard]] CategoriesRec& CategoriesTable() const { return *_categoriesrec; }
    [[nodiscard]] Ingredients& IngredientsTable() const { return *_ingredients; }
    [[nodiscard]] Units& UnitsTable() const { return *_units; }
    [[nodiscard]] Recipes& RecipesTable() const { return *_recipes; }
    [[nodiscard]] Recipeingred& RecipeIngredientsTable() const { return *_recipeingred; }

private:
    QSqlDatabase _db;

    CategoriesRec* _categoriesrec;
    Ingredients* _ingredients;
    Units* _units;
    Recipes* _recipes;
    Recipeingred* _recipeingred;


};

