#include "PageChanger.h"
#include "../Interface/Pages/BookmarksPage.h"
#include "../Interface/Pages/IngredientsPage.h"
#include "../Interface/Pages/MyGroupsPage.h"
#include "../Interface/Pages/ExplorePage.h"
//#include "../Database/databace.h"

PageChanger::PageChanger(ColorScheme &scheme, QRectF rect, DataBase* database)
    : db(database)
{
    HomePage* home = new HomePage(scheme, rect);
    home->setDatabase(db);

    recepie = new RecepiePage(scheme, rect);
    recepie->setDatabase(db);

    BookmarksPage* bookmarks = new BookmarksPage(scheme, rect);
    IngredientsPage* ingredients = new IngredientsPage(scheme, rect);
    MyGroupsPage* myGroups = new MyGroupsPage(scheme, rect);
    ExplorePage* explore = new ExplorePage(scheme, rect);

    pages = { home, recepie, bookmarks, ingredients, myGroups, explore };

    for (auto* page : pages)
        page->refresh();

    connect(home, &HomePage::goToRecipePage,
            this, &PageChanger::openRecipe);
}

QVector<Page*> PageChanger::getPages()
{
    return pages;
}

void PageChanger::resize(int w, int h)
{
    for (auto& page : pages)
        page->resize(w, h);
}

Page* PageChanger::getBasePage()
{
    return pages[0];
}

Page* PageChanger::getCurrentPage(PageID currentPage)
{
    switch (currentPage) {
    case PageID::home: return pages[0];
    case PageID::recepie: return pages[1];
    case PageID::bookmarks: return pages[2];
    case PageID::ingredients: return pages[3];
    case PageID::myGroups: return pages[4];
    case PageID::explore: return pages[5];
    default: return pages[0];
    }
}

void PageChanger::openRecipe(int recipeID)
{
    currentPage = PageID::recepie;

    recepie->setRecipeID(recipeID);
    recepie->refresh();
    pages[0]->refresh();
    emit changePage(PageID::recepie);
}
