#include "categoriesrec.h"
#include <QSqlQuery>
#include <QSqlRecord>
#include <QVariant>

bool CategoriesRec::Read()
{
    if (!IsOpened()) return false;

    QSqlQuery q(_db);
    if (!q.exec("SELECT * FROM Categories")) return false;

    QVector<CategoriesRec> vec;
    QHash<int, CategoriesRec> hash;

    int colId = q.record().indexOf("id");
    int colName = q.record().indexOf("name");

    while (q.next())
    {
        CategoriesRec c;
        c.id = q.value(colId).toInt();
        c.name = q.value(colName).toString();

        vec.append(c);
        hash.insert(c.id, c);
    }

    _vector = vec;
    _hash = hash;

    return true;
}

bool CategoriesRec::Add(const CategoriesRec& c)
{
    if (!IsOpened()) return false;

    DbTransaction tr(_db);
    if (!tr.IsValid()) return false;

    QSqlQuery q(_db);
    q.prepare("INSERT INTO Categories (name) VALUES (:name)");
    q.bindValue(":name", c.name);

    if (!q.exec()) return false;

    tr.Commit();
    return true;
}

bool CategoriesRec::Update(const CategoriesRec& c)
{
    if (!IsOpened()) return false;

    DbTransaction tr(_db);
    if (!tr.IsValid()) return false;

    QSqlQuery q(_db);
    q.prepare("UPDATE Categories SET name = :name WHERE id = :id");
    q.bindValue(":id", c.id);
    q.bindValue(":name", c.name);

    if (!q.exec()) return false;

    tr.Commit();
    return true;
}

bool CategoriesRec::Delete(const CategoriesRec& c)
{
    if (!IsOpened()) return false;

    DbTransaction tr(_db);
    if (!tr.IsValid()) return false;

    QSqlQuery q(_db);
    q.prepare("DELETE FROM Categories WHERE id = :id");
    q.bindValue(":id", c.id);

    if (!q.exec()) return false;

    tr.Commit();
    return true;
}
