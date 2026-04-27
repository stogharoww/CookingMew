#include "Mew.h"
#include "Interface/Virtual/Button.h"
#include "Interface/Interactive/ButtonMew.h"
#include <QGraphicsItem>
#include <QSize>
//#include "Interface/Pages/HomePage.h"
#include <QTimer>
#include <QApplication>
#include "Database/database.h"



Mew::Mew() {
    viewport()->setMouseTracking(true);
    setMouseTracking(true);


    scene = new QGraphicsScene(this);
    setScene(scene);

    scheme = new ColorScheme();

    // ИНИЦИАЛИЗАЦИЯ БАЗЫ
    db = new DataBase(":/databases/resourses/database/CookBook.db");   // путь в ресурсах

    scene->setBackgroundBrush(scheme->backgroundGet());

    initPages();

    QRect screen = QApplication::primaryScreen()->availableGeometry();
    setGeometry(screen);
    setFixedSize(screen.size());
    showMaximized();

    //setUnvisibleAll();

    changeCurrentPage(PageID::home);
    connect(pages->getCurrentPage(currentPg), &Page::changeCurrentPage, this, &Mew::changeCurrentPage);


}



void Mew::meow()
{
    show();
}

void Mew::changeCurrentPage(PageID pageID)
{
    setUnvisibleAll();
    disconnect(pages->getCurrentPage(currentPg), &Page::changeCurrentPage, this, &Mew::changeCurrentPage);
    pages->getCurrentPage(pageID)->setVisible(true);
    currentPg = pageID;
    connect(pages->getCurrentPage(currentPg), &Page::changeCurrentPage, this, &Mew::changeCurrentPage);
    //pages->getCurrentPage(pageID)->changeCurrentPage(pageID);

}

void Mew::recepieCheck(int recID)
{

}



// void Mew::home()
// {
//     setUnvisibleAll();
//     pages->getHomePage()->setVisible(true);
//     //homePage->setVisible(true);
// }

// void Mew::recepie()
// {
//     setUnvisibleAll();
// }



void Mew::resizeEvent(QResizeEvent* event)
{
    QGraphicsView::resizeEvent(event);

    QSize size = event->size();



    //setFixedSize(size);
    scene->setSceneRect(0, 0, size.width(), size.height());

    if (!pages->getPages().isEmpty()){
        pages->resize(size.width(), size.height());
        //homePage->resize(size.width(), size.height());
    }

}

void Mew::setUnvisibleAll()
{
    for (auto& page : pages->getPages()){
        page->setVisible(false);
    }
    //homePage->setVisible(false);
}

void Mew::initPages()
{
    pages = new PageChanger(*scheme, QRectF(0, 0, 100, 100));

    //homePage = new HomePage(*scheme, QRectF(0,0,100,100));
    //page1->update_color_scheme(&scheme);
    for (auto& page : pages->getPages()){
        scene->addItem(page);
    }
    //scene->addItem(homePage);
    connect(pages, &PageChanger::changePage,
            this, &Mew::changeCurrentPage);



}

