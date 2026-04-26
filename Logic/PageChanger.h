#pragma once
#include <QGraphicsItem>
#include "../Interface/ColorScheme.h"
#include "../Interface/Virtual/Page.h"
#include <QVector>
#include "../Interface/Pages/HomePage.h"


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

class PageChanger
{
public:
    PageChanger() {};
    PageChanger(ColorScheme &scheme, QRectF rect);
    QVector<Page*> getPages();
    void resize(int w, int h);

    Page* getBasePage();

    Page* getCurrentPage(PageID currentPage);





private:
    QVector<Page*> pages;
};

