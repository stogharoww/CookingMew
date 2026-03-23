#include "units.h"
#include <QSqlQuery>
#include <QSqlRecord>
#include <QVariant>

bool units::Read()
{
    _db.open();
    if (!IsOpened()) return false;

    QSqlQuery q(_db);
    if (!q.exec("SELECT * FROM Units")) return false;

    QVector<Units> vec;
    QHash<int, Units> hash;

    int colId = q.record().indexOf("id");
    int colName = q.record().indexOf("name");
    int colShort = q.record().indexOf("short_name");

    while (q.next())
    {
        Units u;
        u.id = q.value(colId).toInt();
        u.name = q.value(colName).toString();
        u.short_name = q.value(colShort).toString();

        vec.append(u);
        hash.insert(u.id, u);
    }

    _vector = vec;
    _hash = hash;

    _db.close();
    return true;
}

bool units::Add(const Units& u)
{
    _db.open();
    if (!IsOpened()) return false;

    DbTransaction tr(_db);
    if (!tr.IsValid()) return false;

    QSqlQuery q(_db);
    q.prepare("INSERT INTO Units (name, short_name) VALUES (:name, :short)");

    q.bindValue(":name", u.name);
    q.bindValue(":short", u.short_name);

    if (!q.exec()) return false;

    tr.Commit();
    _db.close();
    return true;
}

bool units::Update(const Units& u)
{
    _db.open();
    if (!IsOpened()) return false;

    DbTransaction tr(_db);
    if (!tr.IsValid()) return false;

    QSqlQuery q(_db);
    q.prepare("UPDATE Units SET name = :name, short_name = :short WHERE id = :id");

    q.bindValue(":id", u.id);
    q.bindValue(":name", u.name);
    q.bindValue(":short", u.short_name);

    if (!q.exec()) return false;

    tr.Commit();
    _db.close();
    return true;
}

bool units::Delete(const Units& u)
{
    _db.open();
    if (!IsOpened()) return false;

    DbTransaction tr(_db);
    if (!tr.IsValid()) return false;

    QSqlQuery q(_db);
    q.prepare("DELETE FROM Units WHERE id = :id");
    q.bindValue(":id", u.id);

    if (!q.exec()) return false;

    tr.Commit();
    _db.close();
    return true;
}
