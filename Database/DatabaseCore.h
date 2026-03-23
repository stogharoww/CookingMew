#pragma once
#include <QSqlDatabase>

/// Базовый класс для всех обработчиков таблиц
class DatabaseCore
{
public:
    DatabaseCore() = default;;
    /// Конструктор принимает подключение к БД
    explicit DatabaseCore(const QSqlDatabase& db) : _db(db) {}

    /// Проверка, что база открыта и валидна
    bool IsOpened() const
    {
        return _db.isValid() && _db.isOpen();
    }

protected:
    /// Подключение к базе данных, доступно наследникам
    QSqlDatabase _db;
};


/// Класс для транзакций
class DbTransaction
{
private:
    QSqlDatabase _db;
    bool _valid;
    bool _committed = false;

public:
    /// Начинаем транзакцию
    explicit DbTransaction(const QSqlDatabase& db) : _db(db)
    {
        _valid = _db.transaction();
    }

    /// Если транзакция не зафиксирована — откатываем
    ~DbTransaction()
    {
        if (!_committed)
            _db.rollback();
    }

    /// Проверка, что транзакция успешно началась
    bool IsValid() const { return _valid; }

    /// Фиксация изменений
    void Commit()
    {
        if (_valid && !_committed)
        {
            _valid = _db.commit();
            _committed = _valid;
        }
    }
};
