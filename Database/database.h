#pragma once
#include <QString>
#include <QSqlDatabase>

#include "categoriesrec.h"
#include "ingredients.h"
#include "units.h"
#include "recipes.h"
#include "recipeingred.h"

class DataBase
{
public:
    explicit DataBase(const QString& dbPath);

    CategoriesRec& CategoriesTable() const { return *_categoriesrec; }
    ingredients& IngredientsTable() const { return *_ingredients; }
    units& UnitsTable() const { return *_units; }
    recipes& RecipesTable() const { return *_recipes; }
    Recipeingred& RecipeIngredientsTable() const { return *_recipeingred; }

private:
    QSqlDatabase _db;

    CategoriesRec* _categoriesrec;
    ingredients* _ingredients;
    units* _units;
    recipes* _recipes;
    Recipeingred* _recipeingred;
};
