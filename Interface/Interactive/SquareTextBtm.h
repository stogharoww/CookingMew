#pragma once

#include "../Virtual/Button.h"
#include <QVector>
#include <QString>
#include <QFont>
#include <QColor>
#include <QGraphicsItem>
#include <QGraphicsRectItem>
#include <QGraphicsTextItem>
#include <QPixmap>
#include "../ColorScheme.h"
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsSceneHoverEvent>
#include <QBrush>


class SquareTextBtm : public Button
{
public:
    SquareTextBtm(ColorScheme& scheme, QGraphicsItem* paren = nullptr);
    void setSize(qreal w, qreal h);
    void setText(QString &text);


private:
    void paint(QPainter* painter, const QStyleOptionGraphicsItem*, QWidget*) override;
    ColorScheme scheme;
    //void setFont();

    QString _text = "";
    QBrush currentBrush = QBrush(scheme.backgroundGet());


    virtual void mousePressEvent(QGraphicsSceneMouseEvent * event) override;
    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent * event) override;
    virtual void hoverEnterEvent(QGraphicsSceneHoverEvent * event) override;
    virtual void hoverLeaveEvent(QGraphicsSceneHoverEvent * event) override;


};


