#pragma once
#include <QGraphicsObject>
#include "../ColorScheme.h"
#include "../Items/IngridientsList.h"
#include "../Virtual/Button.h"
#include "../Interactive/ButtonMew.h"
#include "../Items/Line.h"



enum class PageID{
    home,
    recepie,
    explore,
    bookmarks,
    ingredients,
    myGroups,
    more,
    post
};

class Page : public QGraphicsObject
{
    Q_OBJECT

public:
    Page(ColorScheme& scheme, QRectF rect);

    virtual void resize(int width, int height);

    QRectF boundingRect() const override { return rect; }
    void paint(QPainter*, const QStyleOptionGraphicsItem*, QWidget*) override {}
    //virtual void update_color_scheme(ColorScheme &new_scheme);

signals:
    void changeCurrentPage(PageID currentPage);

protected:

    virtual void create_left_pannel();
    virtual void create_main_pannel() = 0;
    virtual void create_right_pannel() {};


    virtual void update_pages();
    virtual void change_current_btm(QString &btmText);
    virtual void btmHomeClicked();
    virtual void btmExploreClicked();
    virtual void btmBookmarksClicked();
    virtual void btmIngredientsClicked();
    virtual void btmMyGroupsClicked();
    virtual void btmMoreClicked();
    virtual void btmPostClicked();

    virtual void connectBtms();



    ColorScheme& scheme;
    QRectF rect;
    qreal width;
    qreal height;

    // ButtonMew *button;
    // ButtonMew *homeBtm;
    // ButtonMew *exploreBtm;
    // ButtonMew *bookmarksBtm;
    // ButtonMew *ingredientsBtm;
    // ButtonMew *myGroupsBtm;
    // ButtonMew *moreBtm;
    // ButtonMew *postBtm;
    QVector<ButtonMew*> _btms;

    QRectF leftRect;

    QString currentBtm;
    QVector<QString> textForBtm;

};
