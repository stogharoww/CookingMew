#pragma once
#include <QVector>
#include <QHash>
#include "databasecore.h"
#include "Models.h"

class recipes : DatabaseCore
{
public:

    recipes();
    explicit recipes(const QSqlDatabase& db) : DatabaseCore(db) {}

    const QVector<Recipes>& Vector() const { return _vector; }
    const QHash<int, Recipes>& Hash() const { return _hash; }

    bool Read();
    bool Add(const Recipes& item);
    bool Update(const Recipes& item);
    bool Delete(const Recipes& item);

private:
    QVector<Recipes> _vector;
    QHash<int, Recipes> _hash;
    bool IsOpened() const { return _db.isOpen(); }
};
