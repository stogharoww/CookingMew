#include "Ingredients.h"
#include <QSqlQuery>
#include <QSqlRecord>
#include <QVariant>
#include <QVector>

bool ingredients::Read()
{
    _db.open();
    if (!IsOpened()) return false;

    QSqlQuery q(_db);
    if (!q.exec("SELECT * FROM ingredients")) return false;

    QVector<ingredient> vec;
    QHash<int, ingredient> hash;

    int colId = q.record().indexOf("id");
    int colName = q.record().indexOf("name");
    int colProtein = q.record().indexOf("protein");
    int colFat = q.record().indexOf("fat");
    int colCarbs = q.record().indexOf("carbs");
    int colCalories = q.record().indexOf("calories");
    int colCatIng = q.record().indexOf("category_ing_id");

    while (q.next())
    {
        ingredient ing;
        ing.id = q.value(colId).toInt();
        ing.name = q.value(colName).toString();
        ing.protein = q.value(colProtein).toDouble();
        ing.fat = q.value(colFat).toDouble();
        ing.carbs = q.value(colCarbs).toDouble();
        ing.calories = q.value(colCalories).toDouble();
        ing.category_ing_id = q.value(colCatIng).toInt();

        vec.append(ing);
        hash.insert(ing.id, ing);
    }

    _vector = vec;
    _hash = hash;

    _db.close();
    return true;
}

bool ingredients::Add(const ingredient& ing)
{
    _db.open();
    if (!IsOpened()) return false;

    DbTransaction tr(_db);
    if (!tr.IsValid()) return false;

    QSqlQuery q(_db);
    q.prepare("INSERT INTO Ingredients (name, protein, fat, carbs, calories, category_ing_id) "
              "VALUES (:name, :protein, :fat, :carbs, :calories, :cat)");

    q.bindValue(":name", ing.name);
    q.bindValue(":protein", ing.protein);
    q.bindValue(":fat", ing.fat);
    q.bindValue(":carbs", ing.carbs);
    q.bindValue(":calories", ing.calories);
    q.bindValue(":cat", ing.category_ing_id);

    if (!q.exec()) return false;

    tr.Commit();
    _db.close();
    return true;
}

bool ingredients::Update(const ingredient& ing)
{
    _db.open();
    if (!IsOpened()) return false;

    DbTransaction tr(_db);
    if (!tr.IsValid()) return false;

    QSqlQuery q(_db);
    q.prepare("UPDATE Ingredients SET "
              "name = :name, protein = :protein, fat = :fat, carbs = :carbs, calories = :calories, category_ing_id = :cat "
              "WHERE id = :id");

    q.bindValue(":id", ing.id);
    q.bindValue(":name", ing.name);
    q.bindValue(":protein", ing.protein);
    q.bindValue(":fat", ing.fat);
    q.bindValue(":carbs", ing.carbs);
    q.bindValue(":calories", ing.calories);
    q.bindValue(":cat", ing.category_ing_id);

    if (!q.exec()) return false;

    tr.Commit();
    _db.close();
    return true;
}

bool ingredients::Delete(const ingredient& ing)
{
    _db.open();
    if (!IsOpened()) return false;

    DbTransaction tr(_db);
    if (!tr.IsValid()) return false;

    QSqlQuery q(_db);
    q.prepare("DELETE FROM Ingredients WHERE id = :id");
    q.bindValue(":id", ing.id);

    if (!q.exec()) return false;

    tr.Commit();
    _db.close();
    return true;
}
