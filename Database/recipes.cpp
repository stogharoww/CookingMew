#include "Recipes.h"
#include <QSqlQuery>
#include <QSqlRecord>
#include <QVariant>

bool Recipes::Read()
{
    _db.open();
    if (!IsOpened()) return false;

    QSqlQuery q(_db);
    if (!q.exec("SELECT * FROM Recipes")) return false;

    QVector<Recipe> vec;
    QHash<int, Recipe> hash;

    int colId = q.record().indexOf("id");
    int colTitle = q.record().indexOf("title");
    int colInstr = q.record().indexOf("instructions");
    int colCat = q.record().indexOf("category_id");

    while (q.next())
    {
        Recipe r;
        r.id = q.value(colId).toInt();
        r.title = q.value(colTitle).toString();
        r.instructions = q.value(colInstr).toString();
        r.category_id = q.value(colCat).toInt();

        vec.append(r);
        hash.insert(r.id, r);
    }

    _vector = vec;
    _hash = hash;

    _db.close();
    return true;
}

bool Recipes::Add(const Recipe& r)
{
    _db.open();
    if (!IsOpened()) return false;

    DbTransaction tr(_db);
    if (!tr.IsValid()) return false;

    QSqlQuery q(_db);
    q.prepare("INSERT INTO Recipes (title, instructions, category_id) "
              "VALUES (:title, :instr, :cat)");

    q.bindValue(":title", r.title);
    q.bindValue(":instr", r.instructions);
    q.bindValue(":cat", r.category_id);

    if (!q.exec()) return false;

    tr.Commit();
    _db.close();
    return true;
}

bool Recipes::Update(const Recipe& r)
{
    _db.open();
    if (!IsOpened()) return false;

    DbTransaction tr(_db);
    if (!tr.IsValid()) return false;

    QSqlQuery q(_db);
    q.prepare("UPDATE Recipes SET "
              "title = :title, instructions = :instr, category_id = :cat "
              "WHERE id = :id");

    q.bindValue(":id", r.id);
    q.bindValue(":title", r.title);
    q.bindValue(":instr", r.instructions);
    q.bindValue(":cat", r.category_id);

    if (!q.exec()) return false;

    tr.Commit();
    _db.close();
    return true;
}

bool Recipes::Delete(const Recipe& r)
{
    _db.open();
    if (!IsOpened()) return false;

    DbTransaction tr(_db);
    if (!tr.IsValid()) return false;

    QSqlQuery q(_db);
    q.prepare("DELETE FROM Recipes WHERE id = :id");
    q.bindValue(":id", r.id);

    if (!q.exec()) return false;

    tr.Commit();
    _db.close();
    return true;
}
