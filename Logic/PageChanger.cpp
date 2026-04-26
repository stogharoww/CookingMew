#include "PageChanger.h"
#include "../Interface/Pages/RecepiePage.h"


PageChanger::PageChanger(ColorScheme &scheme, QRectF rect)
{
    HomePage *home = new HomePage(scheme, rect);
    RecepiePage *recepie = new RecepiePage(scheme, rect);
    //Page *page = new Page(scheme, rect);

    pages = {
        home, recepie
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
    case PageID::explore: return pages[1];
    default: pages[1];
    };

}




