#pragma once
#include "databasecore.h"
#include <QHash>
#include "Models.h"
#include <QVector>
#include <QString>

class units : DatabaseCore
{
public:
    int id;
    QString name;
    units();
    explicit units(const QSqlDatabase& db) : DatabaseCore(db)
    {
        _db = db;
    }
    [[nodiscard]] const QVector<Units>& Vector() const {return _vector;}
    [[nodiscard]] const QHash<int, Units>& Hash() const {return _hash;}

    bool Read();
    //hello
    bool Add(const Units& item);
    bool Update(const Units& item);
    bool Delete(const Units& item);

private:
    QSqlDatabase _db;
    QVector<Units> _vector;
    QHash<int, Units> _hash;
    bool IsOpened() const { return _db.isOpen(); }
};


