#include "database.h"
#include <QFile>
#include <QDir>
#include <QStandardPaths>
#include <QDebug>
#include <QSqlError>

DataBase::DataBase(const QString &dbPath)
{
    // Папка приложения
    QString appData = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    QDir().mkpath(appData);

    QString fullPath = appData + "/CookBook.db";

    qDebug() << "DB target path:" << fullPath;

    // --- Диагностика существующего файла ---
    if (QFile::exists(fullPath)) {
        QFile f(fullPath);
        qDebug() << "Existing DB size:" << f.size();
    } else {
        qDebug() << "Existing DB does not exist.";
    }

    // --- Диагностика ресурса ---
    qDebug() << "Checking resource path:" << dbPath;
    qDebug() << "Resource exists:" << QFile::exists(dbPath);

    QFile rf(dbPath);
    qDebug() << "Resource DB size:" << rf.size();

    // --- Если файл отсутствует или пустой — пересоздаём ---
    bool needCopy = false;

    if (!QFile::exists(fullPath)) {
        qDebug() << "Target DB does not exist. Will copy.";
        needCopy = true;
    } else {
        QFile f(fullPath);
        if (f.size() < 100) {
            qDebug() << "Target DB is too small. Removing and copying again.";
            QFile::remove(fullPath);
            needCopy = true;
        }
    }

    if (needCopy) {
        if (!QFile::copy(dbPath, fullPath)) {
            qDebug() << "Failed to copy DB from resources!";
        } else {
            qDebug() << "DB copied from resources.";
            QFile::setPermissions(fullPath,
                                  QFileDevice::ReadOwner | QFileDevice::WriteOwner |
                                      QFileDevice::ReadUser  | QFileDevice::WriteUser);
        }
    }

    // --- Открываем базу ---
    _db = QSqlDatabase::addDatabase("QSQLITE", "cookbook_connection");
    _db.setDatabaseName(fullPath);

    if (!_db.open()) {
        qDebug() << "DB open error:" << _db.lastError().text();
    } else {
        qDebug() << "DB opened OK at:" << fullPath;
        qDebug() << "Tables:" << _db.tables();
    }

    // --- Инициализация таблиц ---
    _categoriesrec = new CategoriesRec(_db);
    _ingredients   = new ingredients(_db);
    _units         = new units(_db);
    _recipes       = new recipes(_db);
    _recipeingred  = new Recipeingred(_db);

    // --- ЧТЕНИЕ ТАБЛИЦ ---
    _categoriesrec->Read();
    _ingredients->Read();
    _units->Read();
    _recipes->Read();
    _recipeingred->Read();

    qDebug() << "Loaded:"
             << _recipes->Vector().size() << "recipes,"
             << _ingredients->Vector().size() << "ingredients,"
             << _recipeingred->Vector().size() << "recipe_ingredients.";
}
