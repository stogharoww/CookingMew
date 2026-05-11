#pragma once

#include <QPainter>
#include <QGraphicsObject>
#include <QKeyEvent>
#include <QFont>
#include <QString>
#include <QColor>
#include <QLineEdit>
#include <QGraphicsProxyWidget>
#include "../ColorScheme.h"

class SearchMew: public QGraphicsObject
{
    Q_OBJECT
public:
    SearchMew(ColorScheme& scheme, QGraphicsItem* paren = nullptr);
    QRectF boundingRect() const override;

    void paint(QPainter* painter, const QStyleOptionGraphicsItem*, QWidget*) override;

private:
    ColorScheme _scheme;
    QLineEdit* _input = nullptr;
    QGraphicsProxyWidget* _proxy = nullptr;
    void setupInput();
};


