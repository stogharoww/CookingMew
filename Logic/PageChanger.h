#pragma once
#include <QGraphicsItem>
#include "../Interface/ColorScheme.h"
#include "../Interface/Virtual/Page.h"
#include <QVector>
#include "../Interface/Pages/HomePage.h"
#include "../Interface/Pages/RecepiePage.h"
#include <QObject>
#include "../Database/database.h"
#include "../Interface/Pages/EditRecipePage.h"




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
    void backFromEdit(int recipeID);
    // void createdPage();  // БОЛЬШЕ НЕ НУЖЕН

signals:
    void changePage(PageID id);

private:
    QVector<Page*> pages;
    PageID currentPage = PageID::home;

    RecepiePage* recepie;
    EditRecipePage *editRecipePage;
    HomePage* home;
};


