#include "recipeingred.h"
#include <QSqlQuery>
#include <QSqlRecord>
#include <QVariant>
#include <QSqlError>

bool Recipeingred::Read()
{
    if (!IsOpened()) return false;

    QSqlQuery q(_db);

    // ВАЖНО: имя таблицы как в базе!
    if (!q.exec("SELECT * FROM recipe_ingredients")) {
        qDebug() << "SQL error in Recipeingred::Read:" << q.lastError().text();
        return false;
    }

    QVector<Recipeingred> vec;
    QHash<int, Recipeingred> hash;

    int colRec = q.record().indexOf("recipe_id");
    int colIng = q.record().indexOf("ingredient_id");
    int colAmount = q.record().indexOf("amount");
    int colUnit = q.record().indexOf("unit_id");

    while (q.next())
    {
        Recipeingred ri;
        ri.recipe_id = q.value(colRec).toInt();
        ri.ingredient_id = q.value(colIng).toInt();
        ri.amount = q.value(colAmount).toDouble();
        ri.unit_id = q.value(colUnit).toInt();

        vec.append(ri);

        hash.insert(ri.recipe_id * 100000 + ri.ingredient_id, ri);
    }

    _vector = vec;
    _hash = hash;

    return true;
}

bool Recipeingred::Add(const Recipeingred& ri)
{
    if (!IsOpened()) return false;

    DbTransaction tr(_db);
    if (!tr.IsValid()) return false;

    QSqlQuery q(_db);

    // имя таблицы как в базе
    q.prepare("INSERT INTO recipe_ingredients (recipe_id, ingredient_id, amount, unit_id) "
              "VALUES (:rec, :ing, :amount, :unit)");

    q.bindValue(":rec", ri.recipe_id);
    q.bindValue(":ing", ri.ingredient_id);
    q.bindValue(":amount", ri.amount);
    q.bindValue(":unit", ri.unit_id);

    if (!q.exec()) {
        qDebug() << "SQL error in Recipeingred::Add:" << q.lastError().text();
        return false;
    }

    tr.Commit();
    return true;
}

bool Recipeingred::Update(const Recipeingred& ri)
{
    if (!IsOpened()) return false;

    DbTransaction tr(_db);
    if (!tr.IsValid()) return false;

    QSqlQuery q(_db);

    q.prepare("UPDATE recipe_ingredients SET "
              "amount = :amount, unit_id = :unit "
              "WHERE recipe_id = :rec AND ingredient_id = :ing");

    q.bindValue(":rec", ri.recipe_id);
    q.bindValue(":ing", ri.ingredient_id);
    q.bindValue(":amount", ri.amount);
    q.bindValue(":unit", ri.unit_id);

    if (!q.exec()) {
        qDebug() << "SQL error in Recipeingred::Update:" << q.lastError().text();
        return false;
    }

    tr.Commit();
    return true;
}

bool Recipeingred::Delete(const Recipeingred& ri)
{
    if (!IsOpened()) return false;

    DbTransaction tr(_db);
    if (!tr.IsValid()) return false;

    QSqlQuery q(_db);

    q.prepare("DELETE FROM recipe_ingredients WHERE recipe_id = :rec AND ingredient_id = :ing");

    q.bindValue(":rec", ri.recipe_id);
    q.bindValue(":ing", ri.ingredient_id);

    if (!q.exec()) {
        qDebug() << "SQL error in Recipeingred::Delete:" << q.lastError().text();
        return false;
    }

    tr.Commit();
    return true;
}
