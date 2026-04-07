#pragma once

#include "../Virtual/Button.h"
#include <QPixmap>
#include <QGraphicsRectItem>
#include <QGraphicsItem>
#include <QGraphicsTextItem>
#include <QGraphicsPixmapItem>
#include <QVector>
#include <QColor>

enum class ButtonType {
    Like,
    Comment,
    AddBtm,
    Bookmarks,
    Expand

};

class ButtonIcon : public Button
{
public:
    ButtonIcon(ButtonType type, ColorScheme& scheme,
               QGraphicsItem* parent = nullptr, QColor mainColor = nullptr);


private:
    QColor mainCol;

    QVector<QColor>  colors;
    ButtonType btmType;


    QString pathToIcons;
    bool _fold = false;
    QPixmap iconPixmap;

    QString iconPath();
    void iconAppend();
    void paint(QPainter* painter, const QStyleOptionGraphicsItem*, QWidget*) override;



};


