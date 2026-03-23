#pragma once
#include "databasecore.h"
#include "Models.h"
#include <QHash>
#include <QVector>
#include <QVector>

struct ingredient
{
    int id = 0;
    QString name;
    double protein = 0;
    double fat = 0;
    double carbs = 0;
    double calories = 0;
    int category_ing_id = 0;
};

class ingredients : DatabaseCore
{
public:
    ingredients() = default;
    explicit ingredients(const QSqlDatabase &db) : DatabaseCore(db)
    {
        _db = db;
    }
    [[nodiscard]] const QVector<ingredient>& Vector() const { return _vector;}
    [[nodiscard]] const QHash<int, ingredient>& Hash() const { return _hash; }

    bool Read();
    bool Add(const ingredient& ing);
    bool Update(const ingredient& ing);
    bool Delete(const ingredient& ing);

private:
    QSqlDatabase _db;
    QVector<ingredient> _vector;
    QHash<int, ingredient> _hash;
    bool IsOpened() const { return _db.isOpen(); }
};


