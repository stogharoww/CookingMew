#pragma once
#include <QVector>
#include <QHash>
#include "databasecore.h"
#include "Models.h"

/*struct Recipeingred
{
    int recipe_id;
    int ingredient_id;
    double amount;
    int unit_id;
};*/

class Recipeingred : DatabaseCore
{
public:
    int recipe_id = 0;
    int ingredient_id = 0;
    double amount = 0;
    int unit_id = 0;
    Recipeingred()= default;;
    explicit Recipeingred(const QSqlDatabase& db) : DatabaseCore(db) {}

    const QVector<Recipeingred>& Vector() const { return _vector; }
    const QHash<int, Recipeingred>& Hash() const { return _hash; }

    bool Read();
    bool Add(const Recipeingred& ri);
    bool Update(const Recipeingred& ri);
    bool Delete(const Recipeingred& ri);

private:
    QVector<Recipeingred> _vector;
    QHash<int, Recipeingred> _hash;
    bool IsOpened() const { return _db.isOpen(); }
};
