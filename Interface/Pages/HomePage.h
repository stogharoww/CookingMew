#pragma once

#include "../Virtual/Page.h"
#include <QColor>
#include "../ColorScheme.h"
#include "../Items/IngridientsList.h"
#include "../Virtual/Button.h"
#include "../Interactive/ButtonMew.h"
#include <QVector>
#include <QString>
#include <QGraphicsItemGroup>
#include <QGraphicsItem>
#include "../Items/Line.h"
#include <QRectF>
#include "../Interactive/SquareTextBtm.h"
#include "../Items/FollowLent.h"
#include "../Interactive/MewScrollArea.h"

class HomePage : public Page
{
public:
    HomePage(ColorScheme &scheme, QRectF rect);

public slots:
    void btmForYouClicked();
    void btmFollowClicked();
    void btmHeartClicked();

private:
    void create_main_pannel() override;
    QRectF mainRect;
    QVector<Button*> _btms;
    ColorScheme _scheme;
    SquareTextBtm *forYouBtm;
    SquareTextBtm *followBtm;
    Line *followAndForYouLine;
    QGraphicsRectItem *mainRectItem;

    MewScrollArea *forYouScrollArea;

    void moveLine(SquareTextBtm* btm);

    void connecting();
};

