#pragma once
#include <QGraphicsObject>
#include <QGraphicsRectItem>
#include <QGraphicsTextItem>
#include "../Interactive/ButtonMew.h"
#include "../ColorScheme.h"

class ConfirmDialog : public QGraphicsObject
{
    Q_OBJECT

public:
    ConfirmDialog(ColorScheme& scheme, QGraphicsItem* parent = nullptr);

    void setMessage(const QString& text);

signals:
    void accepted();
    void rejected();

protected:
    QRectF boundingRect() const override;
    void paint(QPainter*, const QStyleOptionGraphicsItem*, QWidget*) override {}

private:
    ColorScheme _scheme;

    QGraphicsRectItem* dimmer = nullptr;
    QGraphicsRectItem* box = nullptr;
    QGraphicsTextItem* message = nullptr;

    ButtonMew* yesBtn = nullptr;
    ButtonMew* noBtn = nullptr;
};
