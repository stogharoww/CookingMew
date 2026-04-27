#pragma once
#include <QGraphicsItem>
#include "../Interface/ColorScheme.h"
#include "../Interface/Virtual/Page.h"
#include <QVector>
#include "../Interface/Pages/HomePage.h"
#include "../Interface/Pages/RecepiePage.h"
#include <QObject>



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

class PageChanger: public QObject
{
    Q_OBJECT
public:
    PageChanger() {};
    PageChanger(ColorScheme &scheme, QRectF rect);
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
    RecepiePage *recepie = nullptr;
};

