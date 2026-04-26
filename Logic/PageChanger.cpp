#include "PageChanger.h"
#include "../Interface/Pages/RecepiePage.h"
#include "../Interface/Pages/BookmarksPage.h".h"
#include "../Interface/Pages/IngredientsPage.h".h"
#include "../Interface/Pages/MyGroupsPage.h".h"
#include "../Interface/Pages/ExplorePage.h".h"


PageChanger::PageChanger(ColorScheme &scheme, QRectF rect)
{
    HomePage *home = new HomePage(scheme, rect);
    RecepiePage *recepie = new RecepiePage(scheme, rect);
    BookmarksPage *bookmarks = new BookmarksPage(scheme, rect);
    IngredientsPage *ingredients = new IngredientsPage(scheme, rect);
    MyGroupsPage *myGroups = new MyGroupsPage(scheme, rect);
    ExplorePage *explore = new ExplorePage(scheme, rect);

    //Page *page = new Page(scheme, rect);

    pages = {
        home, recepie, bookmarks, ingredients, myGroups, explore
    };

}

QVector<Page *> PageChanger::getPages()
{
    return pages;
}

void PageChanger::resize(int w, int h)
{
    for (auto& page : pages){
        page->resize(w, h);
    }
}

Page *PageChanger::getBasePage()
{
    return pages[0];
}



Page *PageChanger::getCurrentPage(PageID currentPage)
{
    switch (currentPage){
    case PageID::home: return pages[0];
    case PageID::recepie: return pages[1];
    case PageID::bookmarks: return pages[2];
    case PageID::ingredients: return pages[3];
    case PageID::myGroups: return pages[4];
    case PageID::explore: return pages[5];
    default: pages[0];
    };

}




