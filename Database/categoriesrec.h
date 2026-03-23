#pragma once
#include "databasecore.h"
#include <QHash>
#include "Models.h"
#include <QVector>
#include <QString>

class CategoriesRec : DatabaseCore
{
public:
    int id = 0;
    QString name;
    CategoriesRec();
    explicit CategoriesRec(const QSqlDatabase& db): DatabaseCore(db)
    {
        _db = db;
    }
    //Massive of Categories
    [[nodiscard]] const QVector<CategoriesRec>& Vector() const {return _vector; }
    [[nodiscard]] const QHash<int, CategoriesRec>& Hash() const {return _hash; }

    //Methods
    bool Read();
    bool Add(const CategoriesRec& item);
    bool Update(const CategoriesRec& item);
    bool Delete(const CategoriesRec& item);
private:
    QSqlDatabase _db;
    QVector<CategoriesRec> _vector;
    QHash<int, CategoriesRec> _hash;
    bool IsOpened() const { return _db.isOpen(); }

};


