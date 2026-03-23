#pragma once
#include <QVector>
#include <QHash>
#include "databasecore.h"
#include "Models.h"

struct recipeingred
{
    int recipe_id;
    int ingredient_id;
    double amount;
    int unit_id;
};

class Recipeingred : DatabaseCore
{
public:
    Recipeingred();
    explicit Recipeingred(const QSqlDatabase& db) : DatabaseCore(db) {}

    const QVector<recipeingred>& Vector() const { return _vector; }
    const QHash<int, recipeingred>& Hash() const { return _hash; }

    bool Read();
    bool Add(const recipeingred& ri);
    bool Update(const recipeingred& ri);
    bool Delete(const recipeingred& ri);

private:
    QVector<Recipeingred> _vector;
    QHash<int, Recipeingred> _hash;
    bool IsOpened() const { return _db.isOpen(); }
};
