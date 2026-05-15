#pragma once
#include <QGraphicsItem>
#include "../Interface/ColorScheme.h"
#include "../Interface/Virtual/Page.h"
#include <QVector>
#include "../Interface/Pages/HomePage.h"
#include "../Interface/Pages/RecepiePage.h"
#include <QObject>
#include "../Database/database.h"



// enum class PageID{
//     home,
//     recepie,
//     explore,
//     bookmarks,
//     ingredients,
//     myGroups,
//     more,
//     post
// };

class PageChanger : public QObject
{
    Q_OBJECT
    DataBase* db;

public:
    PageChanger(ColorScheme &scheme, QRectF rect, DataBase* db);

    QVector<Page*> getPages();
    void resize(int w, int h);

    Page* getBasePage();
    Page* getCurrentPage(PageID currentPage);

public slots:
    void openRecipe(int recipeID);

signals:
    void changePage(PageID id);

private:
    QVector<Page*> pages;
    PageID currentPage = PageID::home;

   // DataBase* db;          // <--- ДОБАВЛЕНО
    RecepiePage* recepie;  // <--- уже было
};

