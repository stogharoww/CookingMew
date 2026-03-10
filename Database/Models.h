#pragma once
#include <QString>



struct Categories_rec
{
    int id;
    QString name;

    Categories_rec() {}
};

struct Ingregients
{
    int id;
    QString name;
    qreal protein;
    qreal fat;
    qreal carbs;
    qreal calories;
    int category_ing_id;

    Ingregients() {}
};

struct Recipe_ingredients
{
    int recipe_id;
    int ingredient_id;
    qreal amount;
    int unit_id;

    Recipe_ingredients() {}
};

struct Recipes
{
    int id;
    QString title;
    QString instructions;
    int category_id;

    Recipes() {}
};

struct Units
{
    int id;
    QString name;
    QString short_name;

    Units() {}
};


